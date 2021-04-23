#pragma once
#include <napi.h>
#include <libdivecomputer/irda.h>
#include "../context.h"
#include "../descriptor.h"
#include "nativetransport.h"

class IRDATransport : public NativeTransport, public Napi::ObjectWrap<IRDATransport>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Value iterate(const Napi::CallbackInfo &);

    IRDATransport(const Napi::CallbackInfo &);
    ~IRDATransport();

    Napi::Value getName(const Napi::CallbackInfo &);
    Napi::Value getAddress(const Napi::CallbackInfo &);
    Napi::Value toString(const Napi::CallbackInfo &);
    Napi::Value open(const Napi::CallbackInfo &);

    dc_status_t getNative(dc_iostream_t **iostream, dc_context_t *ctx);

private:
    dc_irda_device_t *device;
    static Napi::FunctionReference constructor;
};