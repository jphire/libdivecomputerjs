#pragma once
#include <napi.h>
#include <libdivecomputer/usbhid.h>
#include "../context.h"
#include "../descriptor.h"

class USBHIDTransport : public Napi::ObjectWrap<USBHIDTransport>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Value iterate(const Napi::CallbackInfo &);
    static Napi::FunctionReference constructor;

    USBHIDTransport(const Napi::CallbackInfo &);
    ~USBHIDTransport();

    Napi::Value toString(const Napi::CallbackInfo &);
    Napi::Value getPid(const Napi::CallbackInfo &);
    Napi::Value getVid(const Napi::CallbackInfo &);
    Napi::Value open(const Napi::CallbackInfo &);

private:
    dc_usbhid_device_t *device;
};