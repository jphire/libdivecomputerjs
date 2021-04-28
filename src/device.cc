#include "device.h"
#include "errors/DCError.h"
#include "enums/eventtypes.h"
#include "values/eventdata.h"
#include "context.h"
#include "descriptor.h"
#include "iostream.h"

Napi::FunctionReference Device::constructor;

void Device::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "Device",
        {
            InstanceMethod<&Device::setEvents>("setEvents"),
            InstanceMethod<&Device::setFingerprint>("setFingerprint"),
            InstanceMethod<&Device::setCancel>("setCancel"),
            InstanceMethod<&Device::foreach>("foreach"),
        });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Device", func);
}

Device::Device(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<Device>(info)
{
    if (info.Length() == 1 && info[0].IsExternal())
    {
        borrowed = true;
        device = info[0].As<Napi::External<dc_device_t>>().Data();
    }
    else if (info.Length() == 3 && info[0].IsObject() && info[1].IsObject() && info[2].IsObject())
    {
        auto context = Napi::ObjectWrap<Context>::Unwrap(info[0].ToObject());
        auto descriptor = Napi::ObjectWrap<Descriptor>::Unwrap(info[1].ToObject());
        auto iostream = Napi::ObjectWrap<IOStream>::Unwrap(info[2].ToObject());

        borrowed = false;
        auto status = dc_device_open(&device, context->getNative(), descriptor->getNative(), iostream->getNative());
        DCError::AssertSuccess(info.Env(), status);
    }
    else
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments for Device. expected {Context}, {Descriptor}, {IOStream}.");
    }
}

Device::~Device()
{
    if (borrowed == false && device != NULL)
    {
        dc_device_close(device);
        device = NULL;
    }
}

dc_device_t *Device::getNative()
{
    return device;
}

void Device::foreach (const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsFunction())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {function}.");
    }
    foreachCallback = Napi::Persistent(info[0].As<Napi::Function>());

    dc_device_foreach(device, nativeForeachCallback, this);
}

int Device::nativeForeachCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata)
{
    auto device = (Device *)userdata;
    auto diveData = Napi::Buffer<unsigned char>::Copy(device->Env(), data, size);
    auto fingerprintData = Napi::Buffer<unsigned char>::Copy(device->Env(), fingerprint, fsize);

    auto result = device->foreachCallback.Call({
        diveData,
        fingerprintData,
    });
    if (result.IsEmpty() || result.IsUndefined() || result.IsNull())
    {
        return 1;
    }

    if (!result.IsBoolean())
    {
        throw Napi::TypeError::New(device->Env(), "Invalid return value from device.foreach callback function. Expected {undefined} or {boolean}");
    }

    return result.ToBoolean().Value() ? 1 : 0;
}

void Device::setCancel(const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsFunction())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {function}.");
    }
    cancelCallback = Napi::Persistent(info[0].As<Napi::Function>());

    dc_device_set_cancel(
        device,
        &Device::nativeCancelCallback,
        this);
}

int Device::nativeCancelCallback(void *userdata)
{
    auto device = reinterpret_cast<Device *>(userdata);
    auto result = device->cancelCallback.Call({});

    if (result.IsUndefined() || result.IsNull())
    {
        return 0;
    }

    if (!result.IsBoolean())
    {
        throw Napi::TypeError::New(device->Env(), "Return value of cancel callback must be an boolean");
    }

    return result.ToBoolean().Value() ? 1 : 0;
}

void Device::setFingerprint(const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsBuffer())
    {
        throw Napi::TypeError::New(Env(), "Invalid argument, expected {ArrayBuffer}.");
    }

    auto buffer = info[0].As<Napi::Buffer<unsigned char>>();
    dc_device_set_fingerprint(device, buffer.Data(), buffer.ByteLength());
}

void Device::setEvents(const Napi::CallbackInfo &info)
{
    if (info.Length() != 2 || !info[0].IsArray() || !info[1].IsFunction())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {number} and {function}.");
    }

    auto eventsArray = info[0].As<Napi::Array>();
    auto events = translateEvents(info.Env(), eventsArray);

    eventCallback = Napi::Persistent(info[1].As<Napi::Function>());

    dc_device_set_events(device, events, nativeEventCallback, this);
}

void Device::nativeEventCallback(dc_device_t *d, dc_event_type_t event, const void *data, void *userdata)
{
    auto device = (Device *)userdata;
    auto env = device->Env();

    auto eventData = wrapEvent(device->Env(), event, data);
    device->eventCallback.Call({eventData});
}
