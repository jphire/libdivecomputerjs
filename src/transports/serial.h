#pragma once
#include <napi.h>
#include <libdivecomputer/serial.h>
#include "../context.h"
#include "../descriptor.h"
#include "nativetransport.h"

class SerialTransport : public NativeTransport, public Napi::ObjectWrap<SerialTransport>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Value iterate(const Napi::CallbackInfo &);

    SerialTransport(const Napi::CallbackInfo &);
    ~SerialTransport();

    Napi::Value toString(const Napi::CallbackInfo &);
    Napi::Value getName(const Napi::CallbackInfo &);
    Napi::Value open(const Napi::CallbackInfo &);

    dc_status_t getNative(dc_iostream_t **iostream, dc_context_t *ctx);

private:
    dc_serial_device_t *device;
    static Napi::FunctionReference constructor;
};