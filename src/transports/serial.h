#pragma once
#include <napi.h>
#include <libdivecomputer/serial.h>
#include "../context.h"
#include "../descriptor.h"

class SerialTransport : public Napi::ObjectWrap<SerialTransport>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Value iterate(const Napi::CallbackInfo &);

    SerialTransport(const Napi::CallbackInfo &);
    ~SerialTransport();

    Napi::Value toString(const Napi::CallbackInfo &);
    Napi::Value getName(const Napi::CallbackInfo &);
    Napi::Value open(const Napi::CallbackInfo &);

private:
    dc_serial_device_t *device;
    static Napi::FunctionReference constructor;
};