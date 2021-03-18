#include "loglevel.h"

Napi::Object getLogLevels(Napi::Env env)
{
    auto loglevels = Napi::Object::New(env);

    loglevels.Set(Napi::String::New(env, LOGLEVEL_NONE), Napi::String::New(env, LOGLEVEL_NONE));
    loglevels.Set(Napi::String::New(env, LOGLEVEL_ERROR), Napi::String::New(env, LOGLEVEL_ERROR));
    loglevels.Set(Napi::String::New(env, LOGLEVEL_WARNING), Napi::String::New(env, LOGLEVEL_WARNING));
    loglevels.Set(Napi::String::New(env, LOGLEVEL_INFO), Napi::String::New(env, LOGLEVEL_INFO));
    loglevels.Set(Napi::String::New(env, LOGLEVEL_DEBUG), Napi::String::New(env, LOGLEVEL_DEBUG));
    loglevels.Set(Napi::String::New(env, LOGLEVEL_ALL), Napi::String::New(env, LOGLEVEL_ALL));

    return loglevels;
}

dc_loglevel_t translateLogLevel(Napi::String logLevel)
{
    auto loglevelString = logLevel.Utf8Value();

    if (loglevelString == LOGLEVEL_NONE)
    {
        return DC_LOGLEVEL_NONE;
    }

    if (loglevelString == LOGLEVEL_ERROR)
    {
        return DC_LOGLEVEL_ERROR;
    }

    if (loglevelString == LOGLEVEL_WARNING)
    {
        return DC_LOGLEVEL_WARNING;
    }

    if (loglevelString == LOGLEVEL_INFO)
    {
        return DC_LOGLEVEL_INFO;
    }

    if (loglevelString == LOGLEVEL_DEBUG)
    {
        return DC_LOGLEVEL_DEBUG;
    }

    if (loglevelString == LOGLEVEL_ALL)
    {
        return DC_LOGLEVEL_ALL;
    }

    return DC_LOGLEVEL_NONE;
}

Napi::String translateLogLevel(Napi::Env env, dc_loglevel_t logLevel)
{

    if (logLevel == DC_LOGLEVEL_NONE)
    {
        return Napi::String::New(env, LOGLEVEL_NONE);
    }

    if (logLevel == DC_LOGLEVEL_ERROR)
    {
        return Napi::String::New(env, LOGLEVEL_ERROR);
    }

    if (logLevel == DC_LOGLEVEL_WARNING)
    {
        return Napi::String::New(env, LOGLEVEL_WARNING);
    }

    if (logLevel == DC_LOGLEVEL_INFO)
    {
        return Napi::String::New(env, LOGLEVEL_INFO);
    }

    if (logLevel == DC_LOGLEVEL_DEBUG)
    {
        return Napi::String::New(env, LOGLEVEL_DEBUG);
    }

    if (logLevel == DC_LOGLEVEL_ALL)
    {
        return Napi::String::New(env, LOGLEVEL_ALL);
    }

    return Napi::String::New(env, LOGLEVEL_NONE);
}

bool isLogLevel(Napi::String logLevel)
{
    auto logLevelString = logLevel.Utf8Value();
    return logLevelString != LOGLEVEL_NONE &&
           logLevelString != LOGLEVEL_ERROR &&
           logLevelString != LOGLEVEL_WARNING &&
           logLevelString != LOGLEVEL_INFO &&
           logLevelString != LOGLEVEL_DEBUG &&
           logLevelString != LOGLEVEL_ALL;
}
