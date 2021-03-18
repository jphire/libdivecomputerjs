#include "watertype.h"

Napi::Object getAllWaterTypes(Napi::Env env)
{
    auto allTankVolumes = Napi::Object::New(env);

    for (auto volume : ALL_WATER_TYPES)
    {
        allTankVolumes.Set(volume, volume);
    }

    return allTankVolumes;
}

dc_water_t translateWaterType(Napi::Env env, Napi::String watertype)
{
    auto salinityString = watertype.Utf8Value().c_str();

    if (salinityString == WATER_FRESH)
    {
        return DC_WATER_FRESH;
    }

    if (salinityString == WATER_SALT)
    {
        return DC_WATER_SALT;
    }

    char buffer[128];
    snprintf(buffer, 128, "Invalid watertype %s", salinityString);
    throw Napi::TypeError::New(env, buffer);
}

Napi::String translateWaterType(Napi::Env env, dc_water_t watertype)
{
    switch (watertype)
    {
    case DC_WATER_FRESH:
        return Napi::String::New(env, WATER_FRESH);
    case DC_WATER_SALT:
        return Napi::String::New(env, WATER_SALT);
    }

    char buffer[128];
    snprintf(buffer, 128, "Invalid watertype %u", watertype);
    throw Napi::TypeError::New(env, buffer);
}
