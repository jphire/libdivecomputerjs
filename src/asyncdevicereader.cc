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
            InstanceMethod<&AsyncDeviceReader::setEvents>("setEvents"),
            InstanceMethod<&AsyncDeviceReader::setDive>("setDive"),
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

void AsyncDeviceReader::setEvents(const Napi::CallbackInfo &info)
{
    if (!info[0].IsArray())
    {
        throw Napi::TypeError::New(info.Env(), "Expected array of events named as first argument");
    }

    events = translateEvents(info.Env(), info[0].As<Napi::Array>());
    eventCallback = Napi::Persistent(info[1].As<Napi::Function>());
}

void AsyncDeviceReader::setDive(const Napi::CallbackInfo &info)
{
    diveCallback = Napi::Persistent(info[0].As<Napi::Function>());
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
        throw Napi::Error::New(info.Env(), "Unable to start reading without diveCallback, use setDive before reading");
    }

    if (eventCallback == NULL)
    {
        throw Napi::Error::New(info.Env(), "Unable to start reading without events, use setEvents before reading");
    }

    if (transport == NULL)
    {
        throw Napi::Error::New(info.Env(), "Unable to start reading without transport, use setTransport before reading");
    }

    auto callback = info[0].As<Napi::Function>();

    auto reader = new AsyncDeviceReaderWorker(callback);
    printf("ctx\n");
    reader->setContext(context);
    printf("descr\n");
    reader->setDescriptor(descriptor);
    printf("setdive\n");
    reader->setDive(diveCallback.Value());
    printf("setevents\n");
    reader->setEvents(events, eventCallback.Value());
    printf("setTransport\n");
    reader->setTransport(transport.Value());

    printf("read queue\n");
    reader->Queue();
}
