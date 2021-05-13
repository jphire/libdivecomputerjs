#include "asyncdevicereaderworker.h"
#include "iostream.h"
#include "errors/DCError.h"
#include "device.h"
#include "transports/usbhid.h"
#include "enums/eventtypes.h"
#include "enums/loglevel.h"
#include <libdivecomputer/usbhid.h>
#include <chrono>
#include <thread>

void freeFingerprint(fingerprint_t &fingerprint)
{
    if (fingerprint.data == NULL)
    {
        return;
    }

    free((void *)fingerprint.data);
    fingerprint.data = NULL;
    fingerprint.size = 0;
}

void copyFingerprint(fingerprint_t &target, const unsigned char *fpdata, unsigned int fpsize)
{
    if (target.data != NULL)
    {
        freeFingerprint(target);
    }

    auto size = fpsize * sizeof(const unsigned char);
    auto copy = malloc(fpsize);
    memcpy(copy, fpdata, size);

    target.data = (const unsigned char *)copy;
    target.size = fpsize;
}

void setDeviceFingerprint(dc_device_t *const device, const fingerprint_t &fingerprint)
{
    if (fingerprint.data != NULL)
    {
        auto status = dc_device_set_fingerprint(device, fingerprint.data, fingerprint.size);
        DCError::AssertSuccess(status);
    }
}

AsyncDeviceReaderWorker::AsyncDeviceReaderWorker(Napi::Function &callback)
    : AsyncWorker(callback)
{
}

AsyncDeviceReaderWorker::~AsyncDeviceReaderWorker()
{
    context->Unref();
    descriptor->Unref();
    transportRef.Unref();

    freeFingerprint(fingerprint);
}

void AsyncDeviceReaderWorker::setContext(Context *context)
{
    if (this->context != NULL)
    {
        this->context->Unref();
    }

    context->Ref();
    this->context = context;

    if (tsfLogdata != nullptr)
    {
        tsfLogdata.Release();
    }

    tsfLogdata = Napi::ThreadSafeFunction::New(
        Env(),
        context->callback.Value(),
        "async log callback",
        0,
        1);
}

void AsyncDeviceReaderWorker::setDescriptor(Descriptor *descriptor)
{
    if (this->descriptor != NULL)
    {
        this->descriptor->Unref();
    }

    descriptor->Ref();
    this->descriptor = descriptor;
}

void AsyncDeviceReaderWorker::setTransport(const Napi::Object &transport)
{
    if (this->transportRef != NULL)
    {
        this->transportRef.Unref();
    }

    this->transport = Napi::ObjectWrap<NativeTransport>::Unwrap(transport);
    this->transportRef = Napi::Persistent(transport);
}

void AsyncDeviceReaderWorker::setEventsCallback(unsigned int events, const Napi::Function &callback)
{
    this->events = events;
    if (tsfEventdata != NULL)
    {
        tsfEventdata.Release();
    }

    tsfEventdata = Napi::ThreadSafeFunction::New(
        Env(),
        callback,
        "async event callback",
        0,
        1);
}

void AsyncDeviceReaderWorker::setDiveCallback(const Napi::Function &diveCallback)
{
    if (tsfDivedata != NULL)
    {
        tsfDivedata.Release();
    }

    tsfDivedata = Napi::ThreadSafeFunction::New(
        Env(),
        diveCallback,
        "async reader diveCallback",
        0,
        1);
}

void AsyncDeviceReaderWorker::setDeviceCallback(const Napi::Function &deviceCallback)
{
    if (tsfDevicedata != NULL)
    {
        tsfDevicedata.Release();
    }

    tsfDevicedata = Napi::ThreadSafeFunction::New(
        Env(),
        deviceCallback,
        "async reader deviceCallback",
        0,
        1);
}

void AsyncDeviceReaderWorker::OnWorkComplete(Napi::Env env, napi_status status)
{
    Napi::AsyncWorker::OnWorkComplete(env, status);
    tsfDivedata.Release();
    tsfEventdata.Release();
    tsfLogdata.Release();
    tsfDevicedata.Release();
}

void AsyncDeviceReaderWorker::Execute()
{
    dc_status_t status;

    dc_descriptor_t *descriptor = this->descriptor->getNative();

    dc_context_t *context;
    status = dc_context_new(&context);
    dc_context_set_loglevel(context, this->context->getNativeLogLevel()); //this->context->getNativeLogLevel());
    dc_context_set_logfunc(
        context, [](dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *message, void *userdata)
        {
            auto devicereader = (AsyncDeviceReaderWorker *)userdata;
            auto logdata = new Logdata(loglevel, message);
            devicereader->tsfLogdata.BlockingCall(logdata, callWithLogdata);
        },
        this);

    dc_iostream_t *iostream;
    status = transport->getNative(&iostream, context);
    DCError::AssertSuccess(status);

    status = dc_device_open(&device, context, descriptor, iostream);
    DCError::AssertSuccess(status);

    if (fingerprint.data != NULL)
    {
        setDeviceFingerprint(device, fingerprint);
    }

    tsfDevicedata.BlockingCall(device, callWithDevicedata);

    dc_device_set_events(
        device, events, [](dc_device_t *d, dc_event_type_t event, const void *data, void *userdata)
        {
            auto devicereader = (AsyncDeviceReaderWorker *)userdata;
            auto divedata = new Eventdata(event, data);
            devicereader->tsfEventdata.BlockingCall(divedata, callWithEventdata);
        },
        this);

    dc_device_foreach(
        device, [](const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata)
        {
            auto devicereader = (AsyncDeviceReaderWorker *)userdata;
            auto divedata = new Divedata(data, size, fingerprint, fsize);
            devicereader->tsfDivedata.BlockingCall(divedata, callWithDivedata);

            return 1;
        },
        this);

    dc_device_close(device);
    dc_iostream_close(iostream);
    dc_context_free(context);
}

void AsyncDeviceReaderWorker::setFingerprint(const unsigned char *fpdata, unsigned int fsize)
{
    freeFingerprint(fingerprint);
    copyFingerprint(fingerprint, fpdata, fsize);

    if (device != NULL)
    {
        setDeviceFingerprint(device, fingerprint);
    }
}

void AsyncDeviceReaderWorker::callWithDivedata(Napi::Env env, Napi::Function jsCallback, Divedata *data)
{
    auto diveDataBuffer = Napi::Buffer<unsigned char>::Copy(env, data->divedata, data->divedatasize);
    auto fingerprintBuffer = Napi::Buffer<unsigned char>::Copy(env, data->fingerprint, data->fingerprintsize);

    jsCallback.Call({diveDataBuffer, fingerprintBuffer});

    delete data;
}

void AsyncDeviceReaderWorker::callWithEventdata(Napi::Env env, Napi::Function jsCallback, Eventdata *data)
{
    auto value = wrapEvent(env, data->event, data->data);
    jsCallback.Call({value});

    delete data;
}

void AsyncDeviceReaderWorker::callWithLogdata(Napi::Env env, Napi::Function jsCallback, Logdata *data)
{
    jsCallback.Call({
        translateLogLevel(env, data->loglevel),
        Napi::String::New(env, data->message),
    });

    delete data;
}

void AsyncDeviceReaderWorker::callWithDevicedata(Napi::Env env, Napi::Function jsCallback, dc_device_t *data)
{
    auto device = Device::constructor.New({Napi::External<dc_device_t>::New(env, data)});
    jsCallback.Call({device});
}
