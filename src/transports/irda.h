#pragma once
#include <napi.h>
#include <libdivecomputer/irda.h>
#include "../context.h"
#include "../descriptor.h"

class IRDATransport : public Napi::ObjectWrap<IRDATransport>
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

private:
    dc_irda_device_t *device;
    static Napi::FunctionReference constructor;
};