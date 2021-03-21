#include "context.h"
#include "enums/loglevel.h"
#include "enums/transport.h"

void Context::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "Context",
        {
            InstanceAccessor<&Context::getLogLevel, &Context::setLogLevel>("logLevel"),
            InstanceAccessor<&Context::getTransports>("transports"),
            InstanceMethod<&Context::setLogCallback>("onLog"),
            InstanceMethod<&Context::log>("log"),
        });

    exports.Set("Context", func);
}

Context::Context(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<Context>(info)
{
    dc_context_new(&context);
    dc_context_set_loglevel(context, logLevel);
}

Context::~Context()
{
    if (context != NULL)
    {
        dc_context_free(context);
        context = NULL;
    }
}

Napi::Value Context::getLogLevel(const Napi::CallbackInfo &info)
{
    return translateLogLevel(info.Env(), logLevel);
}

void Context::setLogLevel(const Napi::CallbackInfo &info, const Napi::Value &logLevelValue)
{
    if (!logLevelValue.IsString())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid argument given, expected exactly one string argument");
    }

    auto logLevelString = logLevelValue.ToString();
    if (isLogLevel(logLevelString))
    {
        throw Napi::TypeError::New(info.Env(), "Invalid argument given, given value isn't a valid loglevel");
    }

    logLevel = translateLogLevel(info[0].ToString());
    dc_context_set_loglevel(context, logLevel);
}

void Context::setLogCallback(const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsFunction())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid argument given, expected exactly one function argument");
    }

    callback = Napi::Persistent(info[0].As<Napi::Function>());

    dc_context_set_logfunc(
        context,
        [](dc_context_t *dccontext, dc_loglevel_t logLevel, const char *file, unsigned int line, const char *function, const char *message, void *userdata) {
            auto context = (Context *)userdata;
            context->internalCallLogCallback(logLevel, message);
        },
        this);
}

dc_context_t *Context::getNative()
{
    return context;
}

void Context::log(const Napi::CallbackInfo &info)
{
    if (info.Length() != 2 || !info[0].IsString() || !info[1].IsString())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments given, expected exactly two string arguments");
    }

    auto logLevelString = info[0].ToString();
    if (isLogLevel(info[0].ToString()))
    {
        throw Napi::TypeError::New(info.Env(), "Invalid argument given, expected first argument to be a valid loglevel");
    }

    auto message = info[0].ToString().Utf8Value().c_str();
    internalCallLogCallback(translateLogLevel(logLevelString), message);
}

Napi::Value Context::getTransports(const Napi::CallbackInfo &info)
{
    auto array = Napi::Array::New(info.Env());

    auto transports = dc_context_get_transports(context);

    return translateTransports(info.Env(), transports);
}

void Context::internalCallLogCallback(dc_loglevel_t logLevel, const char *message)
{
    if (callback.IsEmpty())
    {
        return;
    }

    auto env = callback.Env();
    callback.Call({
        translateLogLevel(env, logLevel),
        Napi::String::New(env, message),
    });
}
