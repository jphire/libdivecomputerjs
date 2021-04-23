#include "usbhid.h"
#include "../errors/DCError.h"
#include "../iostream.h"

Napi::FunctionReference USBHIDTransport::constructor;

void USBHIDTransport::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "USBHIDTransport",
        {
            StaticMethod<&USBHIDTransport::iterate>("iterate"),
            InstanceAccessor<&USBHIDTransport::getPid>("pid"),
            InstanceAccessor<&USBHIDTransport::getVid>("vid"),
            InstanceMethod<&USBHIDTransport::toString>("toString"),
            InstanceMethod<&USBHIDTransport::open>("open"),
        });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("USBHIDTransport", func);
}

Napi::Value USBHIDTransport::iterate(const Napi::CallbackInfo &info)
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
    status = dc_usbhid_iterator_new(&iterator, context->getNative(), descriptor->getNative());

    DCError::AssertSuccess(env, status);

    dc_usbhid_device_t *device;
    int i = 0;
    while ((status = dc_iterator_next(iterator, &device)) != DC_STATUS_DONE)
    {
        DCError::AssertSuccess(env, status);

        array[i++] = constructor.New({
            Napi::External<dc_usbhid_device_t>::New(env, device),
        });
    }

    dc_iterator_free(iterator);

    return array;
}

USBHIDTransport::USBHIDTransport(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<USBHIDTransport>(info)
{
    if (info.Length() != 1 || !info[0].IsExternal())
    {
        throw Napi::TypeError::New(info.Env(), "Cannot construct Descriptor from JS. Use USBHIDTransport.iterate() to iterate all available.");
    }

    this->device = info[0].As<Napi::External<dc_usbhid_device_t>>().Data();
}

USBHIDTransport::~USBHIDTransport()
{
    if (device)
    {
        dc_usbhid_device_free(device);
        device = NULL;
    }
}

Napi::Value USBHIDTransport::getVid(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), dc_usbhid_device_get_vid(device));
}

Napi::Value USBHIDTransport::getPid(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), dc_usbhid_device_get_pid(device));
}

Napi::Value USBHIDTransport::open(const Napi::CallbackInfo &info)
{

    if (info.Length() != 1 || !info[0].IsObject())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid argument, expected {context}.");
    }

    auto context = Napi::ObjectWrap<Context>::Unwrap(info[0].As<Napi::Object>());

    dc_iostream_t *iostream;
    auto status = getNative(&iostream, context->getNative());
    DCError::AssertSuccess(info.Env(), status);

    return IOStream::constructor.New(
        {
            Napi::External<dc_iostream_t>::New(info.Env(), iostream),
        });
}

Napi::Value USBHIDTransport::toString(const Napi::CallbackInfo &info)
{
    char buffer[128];
    snprintf(buffer, 128, "PID: %u; VID: %u", dc_usbhid_device_get_pid(device), dc_usbhid_device_get_vid(device));

    return Napi::String::New(info.Env(), buffer);
}

dc_status_t USBHIDTransport::getNative(dc_iostream_t **iostream, dc_context_t *ctx)
{
    return dc_usbhid_open(iostream, ctx, device);
}