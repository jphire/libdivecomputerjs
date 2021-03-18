#include "device.h"
#include "errors/DCError.h"
#include "enums/eventtypes.h"
#include "context.h"
#include "descriptor.h"
#include "iostream.h"

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

    exports.Set("Device", func);
}

Device::Device(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<Device>(info)
{
    if (info.Length() != 3 || !info[0].IsObject() || !info[1].IsObject())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {context}, {description} and {iostream}.");
    }

    auto context = Napi::ObjectWrap<Context>::Unwrap(info[0].ToObject());
    auto descriptor = Napi::ObjectWrap<Descriptor>::Unwrap(info[1].ToObject());
    auto iostream = Napi::ObjectWrap<IOStream>::Unwrap(info[2].ToObject());

    auto status = dc_device_open(&device, context->getNative(), descriptor->getNative(), iostream->getNative());
    DCError::AssertSuccess(info.Env(), status);
}

Device::~Device()
{
    dc_device_close(device);
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
    auto diveData = Napi::ArrayBuffer::New(device->Env(), (void *)data, size);
    auto fingerprintData = Napi::ArrayBuffer::New(device->Env(), (void *)fingerprint, fsize);

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
    eventCallback = Napi::Persistent(info[1].As<Napi::Function>());

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

    if (!result.IsNumber())
    {
        throw Napi::TypeError::New(device->Env(), "Return value of cancel callback must be an integer number");
    }

    return result.ToNumber().Int32Value();
}

void Device::setFingerprint(const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsArrayBuffer())
    {
        throw Napi::TypeError::New(Env(), "Invalid argument, expected {ArrayBuffer}.");
    }

    auto buffer = info[0].As<Napi::ArrayBuffer>();
    dc_device_set_fingerprint(device, (const unsigned char *)buffer.Data(), buffer.ByteLength());
}

void Device::setEvents(const Napi::CallbackInfo &info)
{
    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsFunction())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {number} and {function}.");
    }

    auto events = info[0].ToNumber().Uint32Value();
    eventCallback = Napi::Persistent(info[1].As<Napi::Function>());

    dc_device_set_events(device, events, nativeEventCallback, this);
}

Napi::Value wrapClockEventData(Napi::Env env, const dc_event_clock_t *clock)
{
    auto clockObject = Napi::Object::New(env);

    clockObject.Set("devtime", clock->devtime);
    clockObject.Set("systime", clock->systime);

    return clockObject;
}

Napi::Value wrapProgressEventData(Napi::Env env, const dc_event_progress_t *progress)
{
    auto progressObject = Napi::Object::New(env);

    progressObject.Set("current", progress->current);
    progressObject.Set("maximum", progress->maximum);

    return progressObject;
}

Napi::Value wrapVendorEventData(Napi::Env env, const dc_event_vendor_t *vendor)
{
    return Napi::ArrayBuffer::New(env, const_cast<unsigned char *>(vendor->data), vendor->size);
}

Napi::Value wrapDevInfoEventData(Napi::Env env, const dc_event_devinfo_t *devinfo)
{
    auto progressObject = Napi::Object::New(env);

    progressObject.Set("firmware", devinfo->firmware);
    progressObject.Set("model", devinfo->model);
    progressObject.Set("serial", devinfo->serial);

    return progressObject;
}

void Device::nativeEventCallback(dc_device_t *d, dc_event_type_t event, const void *data, void *userdata)
{
    auto device = (Device *)userdata;
    auto env = device->Env();

    switch (event)
    {
    case DC_EVENT_CLOCK:
        device->eventCallback.Call({
            Napi::String::New(env, EVENT_CLOCK),
            wrapClockEventData(env, reinterpret_cast<const dc_event_clock_t *>(data)),
        });
        break;

    case DC_EVENT_PROGRESS:
        device->eventCallback.Call({
            Napi::String::New(env, EVENT_PROGRESS),
            wrapProgressEventData(env, reinterpret_cast<const dc_event_progress_t *>(data)),
        });
        break;

    case DC_EVENT_WAITING:
        device->eventCallback.Call({
            Napi::String::New(env, EVENT_WAITING),
        });
        break;

    case DC_EVENT_VENDOR:
        device->eventCallback.Call({
            Napi::String::New(env, EVENT_VENDOR),
            wrapVendorEventData(env, reinterpret_cast<const dc_event_vendor_t *>(data)),
        });
        break;

    case DC_EVENT_DEVINFO:
        auto devinfoData = (dc_event_devinfo_t *)data;
        device->eventCallback.Call({
            Napi::String::New(env, EVENT_DEVINFO),
            wrapDevInfoEventData(env, reinterpret_cast<const dc_event_devinfo_t *>(data)),
        });
        break;
    }
}
