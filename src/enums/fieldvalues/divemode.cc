#include "divemode.h"

Napi::Object getAllDiveModes(Napi::Env env)
{
    auto allDiveModes = Napi::Object::New(env);

    for (auto divemode : ALL_DIVEMODES)
    {
        allDiveModes.Set(divemode, divemode);
    }

    return allDiveModes;
}

dc_divemode_t translateDiveMode(Napi::Env env, Napi::String divemode)
{
    auto divemodeString = divemode.Utf8Value().c_str();

    if (divemodeString == DIVEMODE_FREEDIVE)
    {
        return DC_DIVEMODE_FREEDIVE;
    }

    if (divemodeString == DIVEMODE_GAUGE)
    {
        return DC_DIVEMODE_GAUGE;
    }

    if (divemodeString == DIVEMODE_OC)
    {
        return DC_DIVEMODE_OC;
    }

    if (divemodeString == DIVEMODE_CCR)
    {
        return DC_DIVEMODE_CCR;
    }

    if (divemodeString == DIVEMODE_SCR)
    {
        return DC_DIVEMODE_SCR;
    }

    char buffer[128];
    snprintf(buffer, 128, "Invalid divemode %s", divemodeString);
    throw Napi::TypeError::New(env, buffer);
}

Napi::String translateDiveMode(Napi::Env env, dc_divemode_t divemode)
{
    switch (divemode)
    {
    case DC_DIVEMODE_FREEDIVE:
        return Napi::String::New(env, DIVEMODE_FREEDIVE);
    case DC_DIVEMODE_GAUGE:
        return Napi::String::New(env, DIVEMODE_GAUGE);
    case DC_DIVEMODE_OC:
        return Napi::String::New(env, DIVEMODE_OC);
    case DC_DIVEMODE_CCR:
        return Napi::String::New(env, DIVEMODE_CCR);
    case DC_DIVEMODE_SCR:
        return Napi::String::New(env, DIVEMODE_SCR);
    }

    char buffer[128];
    snprintf(buffer, 128, "Invalid divemode %u", divemode);
    throw Napi::TypeError::New(env, buffer);
}