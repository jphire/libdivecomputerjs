#include "irda.h"
#include "../errors/DCError.h"
#include "../iostream.h"

Napi::FunctionReference IRDATransport::constructor;

void IRDATransport::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "IRDATransport",
        {
            StaticMethod<&IRDATransport::iterate>("iterate"),
            InstanceAccessor<&IRDATransport::getAddress>("address"),
            InstanceAccessor<&IRDATransport::getName>("name"),
            InstanceMethod<&IRDATransport::toString>("toString"),

        });

    constructor = Napi::Persistent(func);

    exports.Set("IRDATransport", func);
}

Napi::Value IRDATransport::iterate(const Napi::CallbackInfo &info)
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
    status = dc_irda_iterator_new(&iterator, context->getNative(), descriptor->getNative());

    DCError::AssertSuccess(env, status);

    dc_irda_device_t *device;
    int i = 0;
    while ((status = dc_iterator_next(iterator, &device)) != DC_STATUS_DONE)
    {
        DCError::AssertSuccess(env, status);

        array[i++] = constructor.New({
            Napi::External<dc_irda_device_t>::New(env, device),
        });
    }

    dc_iterator_free(iterator);

    return array;
}

IRDATransport::IRDATransport(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<IRDATransport>(info)
{
    if (info.Length() != 1 || !info[0].IsExternal())
    {
        throw Napi::TypeError::New(info.Env(), "Cannot construct Descriptor from JS. Use IRDATransport.iterate() to iterate all available.");
    }

    this->device = info[0].As<Napi::External<dc_irda_device_t>>().Data();
}

IRDATransport::~IRDATransport()
{
    if (device)
    {
        dc_irda_device_free(device);
        device = NULL;
    }
}

Napi::Value IRDATransport::open(const Napi::CallbackInfo &info)
{

    if (info.Length() != 1 || info[0].IsObject())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid argument,  expected {context}.");
    }

    auto context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());
    auto address = dc_irda_device_get_address(device);

    dc_iostream_t *iostream;
    auto status = dc_irda_open(&iostream, context->getNative(), address, 1);
    DCError::AssertSuccess(info.Env(), status);

    return IOStream::constructor.New(
        {
            Napi::External<dc_iostream_t>::New(info.Env(), iostream),
        });
}

Napi::Value IRDATransport::getName(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), dc_irda_device_get_name(device));
}

Napi::Value IRDATransport::getAddress(const Napi::CallbackInfo &info)
{
    char buffer[32];
    snprintf(buffer, 32, "%08x", dc_irda_device_get_address(device));
    return Napi::String::New(info.Env(), buffer);
}

Napi::Value IRDATransport::toString(const Napi::CallbackInfo &info)
{
    char buffer[128];
    snprintf(buffer, 128, "%08x %s", dc_irda_device_get_address(device), dc_irda_device_get_name(device));
    return Napi::String::New(info.Env(), buffer);
}