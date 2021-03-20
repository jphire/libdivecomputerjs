#include "sampletypes.h"

Napi::Object getAllSampleTypes(Napi::Env env)
{
    auto types = ALL_SAMPLE_TYPES;
    auto allTypes = Napi::Object::New(env);

    for (auto type : types)
    {
        allTypes.Set(type, type);
    }

    return allTypes;
}

Napi::String translateSampleType(Napi::Env env, dc_sample_type_t sampleType)
{

    switch (sampleType)
    {
    case DC_SAMPLE_BEARING:
        return Napi::String::New(env, SAMPLE_BEARING);
    case DC_SAMPLE_CNS:
        return Napi::String::New(env, SAMPLE_CNS);
    case DC_SAMPLE_DECO:
        return Napi::String::New(env, SAMPLE_DECO);
    case DC_SAMPLE_DEPTH:
        return Napi::String::New(env, SAMPLE_DEPTH);
    case DC_SAMPLE_EVENT:
        return Napi::String::New(env, SAMPLE_EVENT);
    case DC_SAMPLE_GASMIX:
        return Napi::String::New(env, SAMPLE_GASMIX);
    case DC_SAMPLE_HEARTBEAT:
        return Napi::String::New(env, SAMPLE_HEARTBEAT);
    case DC_SAMPLE_PPO2:
        return Napi::String::New(env, SAMPLE_PPO2);
    case DC_SAMPLE_PRESSURE:
        return Napi::String::New(env, SAMPLE_PRESSURE);
    case DC_SAMPLE_RBT:
        return Napi::String::New(env, SAMPLE_RBT);
    case DC_SAMPLE_SETPOINT:
        return Napi::String::New(env, SAMPLE_SETPOINT);
    case DC_SAMPLE_TEMPERATURE:
        return Napi::String::New(env, SAMPLE_TEMPERATURE);
    case DC_SAMPLE_TIME:
        return Napi::String::New(env, SAMPLE_TIME);
    case DC_SAMPLE_VENDOR:
        return Napi::String::New(env, SAMPLE_VENDOR);
    }

    char buffer[128];
    snprintf(buffer, 128, "Invalid sample type: %u", sampleType);
    throw Napi::TypeError::New(env, buffer);
}

dc_sample_type_t translateSampleType(Napi::Env env, Napi::String sampleType)
{
    auto sampleTypeString = sampleType.Utf8Value().c_str();

    if (sampleTypeString == SAMPLE_BEARING)
    {
        return DC_SAMPLE_BEARING;
    }

    if (sampleTypeString == SAMPLE_CNS)
    {
        return DC_SAMPLE_CNS;
    }

    if (sampleTypeString == SAMPLE_DECO)
    {
        return DC_SAMPLE_DECO;
    }

    if (sampleTypeString == SAMPLE_DEPTH)
    {
        return DC_SAMPLE_DEPTH;
    }

    if (sampleTypeString == SAMPLE_EVENT)
    {
        return DC_SAMPLE_EVENT;
    }

    if (sampleTypeString == SAMPLE_GASMIX)
    {
        return DC_SAMPLE_GASMIX;
    }

    if (sampleTypeString == SAMPLE_HEARTBEAT)
    {
        return DC_SAMPLE_HEARTBEAT;
    }

    if (sampleTypeString == SAMPLE_PPO2)
    {
        return DC_SAMPLE_PPO2;
    }

    if (sampleTypeString == SAMPLE_PRESSURE)
    {
        return DC_SAMPLE_PRESSURE;
    }

    if (sampleTypeString == SAMPLE_RBT)
    {
        return DC_SAMPLE_RBT;
    }

    if (sampleTypeString == SAMPLE_SETPOINT)
    {
        return DC_SAMPLE_SETPOINT;
    }

    if (sampleTypeString == SAMPLE_TEMPERATURE)
    {
        return DC_SAMPLE_TEMPERATURE;
    }

    if (sampleTypeString == SAMPLE_TIME)
    {
        return DC_SAMPLE_TIME;
    }

    if (sampleTypeString == SAMPLE_VENDOR)
    {
        return DC_SAMPLE_VENDOR;
    }

    char buffer[128];
    sprintf(buffer, "Invalid sample types %s", sampleTypeString);
    throw Napi::TypeError::New(env, buffer);
}