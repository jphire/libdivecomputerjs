#include "serial.h"
#include "../errors/DCError.h"
#include "../iostream.h"

Napi::FunctionReference SerialTransport::constructor;

void SerialTransport::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "SerialTransport",
        {
            StaticMethod<&SerialTransport::iterate>("iterate"),
            InstanceAccessor<&SerialTransport::getName>("name"),
            InstanceMethod<&SerialTransport::toString>("toString"),

        });

    constructor = Napi::Persistent(func);

    exports.Set("SerialTransport", func);
}

Napi::Value SerialTransport::iterate(const Napi::CallbackInfo &info)
{
    dc_status_t status;
    auto env = info.Env();

    if (info.Length() != 2 || info[0].IsExternal() || info[1].IsExternal())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments,  expected {context} and {descriptor} as arguments");
    }

    auto context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
    auto descriptor = Napi::ObjectWrap<Descriptor>::Unwrap(info[1].As<Napi::Object>());

    auto array = Napi::Array::New(env);

    dc_iterator_t *iterator;
    status = dc_serial_iterator_new(&iterator, context->getNative(), descriptor->getNative());

    DCError::AssertSuccess(env, status);

    dc_serial_device_t *device;
    int i = 0;
    while ((status = dc_iterator_next(iterator, &device)) != DC_STATUS_DONE)
    {
        DCError::AssertSuccess(env, status);

        array[i++] = constructor.New({
            Napi::External<dc_serial_device_t>::New(env, device),
        });
    }

    dc_iterator_free(iterator);

    return array;
}

SerialTransport::SerialTransport(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<SerialTransport>(info)
{
    if (info.Length() != 1 || !info[0].IsExternal())
    {
        throw Napi::TypeError::New(info.Env(), "Cannot construct Descriptor from JS. Use SerialTransport.iterate() to iterate all available.");
    }

    this->device = info[0].As<Napi::External<dc_serial_device_t>>().Data();
}

SerialTransport::~SerialTransport()
{
    if (device)
    {
        dc_serial_device_free(device);
        device = NULL;
    }
}

Napi::Value SerialTransport::getName(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), dc_serial_device_get_name(device));
}

Napi::Value SerialTransport::toString(const Napi::CallbackInfo &info)
{
    return getName(info);
}

Napi::Value SerialTransport::open(const Napi::CallbackInfo &info)
{

    if (info.Length() != 1 || info[0].IsObject())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid argument,  expected {context}.");
    }

    auto context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());

    auto name = dc_serial_device_get_name(device);
    dc_iostream_t *iostream;
    auto status = dc_serial_open(&iostream, context->getNative(), name);
    DCError::AssertSuccess(info.Env(), status);

    return IOStream::constructor.New(
        {
            Napi::External<dc_iostream_t>::New(info.Env(), iostream),
        });
}