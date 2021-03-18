#include "tankvolume.h"

Napi::Object getAllTankVolumes(Napi::Env env)
{
    auto allTankVolumes = Napi::Object::New(env);

    for (auto volume : ALL_TANKVOLUMES)
    {
        allTankVolumes.Set(volume, volume);
    }

    return allTankVolumes;
}

dc_tankvolume_t translateTankVolume(Napi::Env env, Napi::String tankVolume)
{
    auto tankVolumeString = tankVolume.Utf8Value().c_str();

    if (tankVolumeString == TANKVOLUME_NONE)
    {
        return DC_TANKVOLUME_NONE;
    }

    if (tankVolumeString == TANKVOLUME_METRIC)
    {
        return DC_TANKVOLUME_METRIC;
    }

    if (tankVolumeString == TANKVOLUME_IMPERIAL)
    {
        return DC_TANKVOLUME_IMPERIAL;
    }

    char buffer[128];
    snprintf(buffer, 128, "Invalid tank volume %s", tankVolumeString);
    throw Napi::TypeError::New(env, buffer);
}

Napi::String translateTankVolume(Napi::Env env, dc_tankvolume_t tankvolume)
{
    switch (tankvolume)
    {
    case DC_TANKVOLUME_IMPERIAL:
        return Napi::String::New(env, TANKVOLUME_IMPERIAL);
    case DC_TANKVOLUME_METRIC:
        return Napi::String::New(env, TANKVOLUME_METRIC);
    case DC_TANKVOLUME_NONE:
        return Napi::String::New(env, TANKVOLUME_NONE);
    }

    char buffer[128];
    snprintf(buffer, 128, "Invalid tankvolume %u", tankvolume);
    throw Napi::TypeError::New(env, buffer);
}