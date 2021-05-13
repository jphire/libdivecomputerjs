#include "asyncdevicereader.h"
#include "asyncdevicereaderworker.h"
#include "enums/eventtypes.h"

void AsyncDeviceReader::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "AsyncDeviceReader",
        {
            InstanceMethod<&AsyncDeviceReader::setDescriptor>("setDescriptor"),
            InstanceMethod<&AsyncDeviceReader::setContext>("setContext"),
            InstanceMethod<&AsyncDeviceReader::setTransport>("setTransport"),
            InstanceMethod<&AsyncDeviceReader::setEventsCallback>("onEvents"),
            InstanceMethod<&AsyncDeviceReader::setDiveCallback>("onDive"),
            InstanceMethod<&AsyncDeviceReader::setDeviceCallback>("onDevice"),
            InstanceMethod<&AsyncDeviceReader::setFingerprint>("setFingerprint"),
            InstanceMethod<&AsyncDeviceReader::read>("read"),

        });

    exports.Set("AsyncDeviceReader", func);
}

AsyncDeviceReader::AsyncDeviceReader(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<AsyncDeviceReader>(info)
{
}

void AsyncDeviceReader::setDescriptor(const Napi::CallbackInfo &info)
{
    descriptor = Napi::ObjectWrap<Descriptor>::Unwrap(info[0].ToObject());
}

void AsyncDeviceReader::setContext(const Napi::CallbackInfo &info)
{
    context = Napi::ObjectWrap<Context>::Unwrap(info[0].ToObject());
}

void AsyncDeviceReader::setTransport(const Napi::CallbackInfo &info)
{
    transport = Napi::Persistent(info[0].ToObject());
}

void AsyncDeviceReader::setEventsCallback(const Napi::CallbackInfo &info)
{
    if (!info[0].IsArray())
    {
        throw Napi::TypeError::New(info.Env(), "Expected array of events named as first argument");
    }

    events = translateEvents(info.Env(), info[0].As<Napi::Array>());
    eventCallback = Napi::Persistent(info[1].As<Napi::Function>());
}

void AsyncDeviceReader::setDiveCallback(const Napi::CallbackInfo &info)
{
    diveCallback = Napi::Persistent(info[0].As<Napi::Function>());
}

void AsyncDeviceReader::setDeviceCallback(const Napi::CallbackInfo &info)
{
    deviceCallback = Napi::Persistent(info[0].As<Napi::Function>());
}

void AsyncDeviceReader::setFingerprint(const Napi::CallbackInfo &info)
{
    if (!info[0].IsBuffer())
    {
        throw Napi::TypeError::New(info.Env(), "Expected first argument to be a buffer");
    }
    auto value = info[0].As<Napi::Buffer<unsigned char>>();
    fingerprint = Napi::Persistent(value);

    if (worker != NULL)
    {
        worker->setFingerprint(fingerprint.Value().Data(), fingerprint.Value().Length());
    }
}

void AsyncDeviceReader::read(const Napi::CallbackInfo &info)
{
    if (context == NULL)
    {
        throw Napi::Error::New(info.Env(), "Unable to start reading without context, use setContext before reading");
    }

    if (descriptor == NULL)
    {
        throw Napi::Error::New(info.Env(), "Unable to start reading without descriptor, use setDescriptor before reading");
    }

    if (diveCallback == NULL)
    {
        throw Napi::Error::New(info.Env(), "Unable to start reading without diveCallback, use setDiveCallback before reading");
    }

    if (eventCallback == NULL)
    {
        throw Napi::Error::New(info.Env(), "Unable to start reading without events, use setEventsCallback before reading");
    }

    if (deviceCallback == NULL)
    {
        throw Napi::Error::New(info.Env(), "Unable to start reading without events, use setEventsCallback before reading");
    }

    if (transport == NULL)
    {
        throw Napi::Error::New(info.Env(), "Unable to start reading without transport, use setTransport before reading");
    }

    auto callback = info[0].As<Napi::Function>();

    worker = new AsyncDeviceReaderWorker(callback);
    worker->setContext(context);
    worker->setDescriptor(descriptor);
    worker->setDiveCallback(diveCallback.Value());
    worker->setDeviceCallback(deviceCallback.Value());
    worker->setEventsCallback(events, eventCallback.Value());
    worker->setTransport(transport.Value());

    if (!fingerprint.IsEmpty())
    {
        worker->setFingerprint(fingerprint.Value().Data(), fingerprint.Value().Length());
    }

    worker->Queue();
}
