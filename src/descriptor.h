#pragma once

#include <napi.h>
#include <libdivecomputer/descriptor.h>

class Descriptor : public Napi::ObjectWrap<Descriptor>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::FunctionReference constructor;
    static Napi::Value iterate(const Napi::CallbackInfo &info);

    Descriptor(const Napi::CallbackInfo &);
    ~Descriptor();
    Napi::Value getProduct(const Napi::CallbackInfo &);
    Napi::Value getVendor(const Napi::CallbackInfo &);
    Napi::Value getModel(const Napi::CallbackInfo &);
    Napi::Value getTransports(const Napi::CallbackInfo &);

    dc_descriptor_t *getNative();

private:
    dc_descriptor_t *descriptor;
};