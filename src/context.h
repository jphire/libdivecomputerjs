#pragma once

#include <libdivecomputer/context.h>
#include <napi.h>

class Context : public Napi::ObjectWrap<Context>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);

    Context(const Napi::CallbackInfo &info);
    ~Context();
    void setLogLevel(const Napi::CallbackInfo &info, const Napi::Value &logLevel);
    dc_loglevel_t getNativeLogLevel();
    Napi::Value getLogLevel(const Napi::CallbackInfo &info);
    Napi::Value getTransports(const Napi::CallbackInfo &info);
    void setLogCallback(const Napi::CallbackInfo &info);
    void log(const Napi::CallbackInfo &info);

    void internalCallLogCallback(dc_loglevel_t logLevel, const char *msg);
    dc_context_t *getNative();
    Napi::FunctionReference callback;

private:
    dc_context_t *context = NULL;
    dc_loglevel_t logLevel = DC_LOGLEVEL_ERROR;

    Napi::String getLogLevel(Napi::CallbackInfo);
    void setLogLevel(Napi::CallbackInfo);
    void setLogCallback(Napi::CallbackInfo);
};