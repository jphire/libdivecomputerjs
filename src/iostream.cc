#include "iostream.h"

Napi::FunctionReference IOStream::constructor;

void IOStream::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "IOStream",
        {});
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
}

IOStream::IOStream(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<IOStream>(info)
{
    if (info.Length() != 1 || !info[0].IsExternal())
    {
        throw Napi::TypeError::New(info.Env(), "Unable to construct IOStream. Transports are created by calling your transport type's open method (eg. USBHIDTransport.open())");
    }
    iostream = info[0].As<Napi::External<dc_iostream_t>>().Data();
}

IOStream::~IOStream()
{
    dc_iostream_close(iostream);
}

dc_iostream_t *IOStream::getNative()
{
    return iostream;
}