#include "eventsampletype.h"
#include <napi.h>

#define EVENT_TYPE_COUNT 26
const char *event_type_names[EVENT_TYPE_COUNT] = {
    SAMPLE_EVENT_TYPE_NONE,
    SAMPLE_EVENT_TYPE_DECO,
    SAMPLE_EVENT_TYPE_RBT,
    SAMPLE_EVENT_TYPE_ASCENT,
    SAMPLE_EVENT_TYPE_CEILING,
    SAMPLE_EVENT_TYPE_WORKLOAD,
    SAMPLE_EVENT_TYPE_TRANSMITTER,
    SAMPLE_EVENT_TYPE_VIOLATION,
    SAMPLE_EVENT_TYPE_BOOKMARK,
    SAMPLE_EVENT_TYPE_SURFACE,
    SAMPLE_EVENT_TYPE_SAFETY_STOP,
    SAMPLE_EVENT_TYPE_GASCHANGE,
    SAMPLE_EVENT_TYPE_SAFETY_STOP_VOLUNTARY,
    SAMPLE_EVENT_TYPE_SAFETY_STOP_MANDATORY,
    SAMPLE_EVENT_TYPE_DEEPSTOP,
    SAMPLE_EVENT_TYPE_CEILING_SAFETY_STOP,
    SAMPLE_EVENT_TYPE_FLOOR,
    SAMPLE_EVENT_TYPE_DIVETIME,
    SAMPLE_EVENT_TYPE_MAXDEPTH,
    SAMPLE_EVENT_TYPE_OLF,
    SAMPLE_EVENT_TYPE_PO2,
    SAMPLE_EVENT_TYPE_AIRTIME,
    SAMPLE_EVENT_TYPE_RGBM,
    SAMPLE_EVENT_TYPE_HEADING,
    SAMPLE_EVENT_TYPE_TISSUE_LEVEL_WARNING,
    SAMPLE_EVENT_TYPE_GASCHANGE2,
};

#define DECO_TYPE_COUNT 4
const char *deco_type_names[EVENT_TYPE_COUNT] = {
    DECO_TYPE_NDL,
    DECO_TYPE_SAFETYSTOP,
    DECO_TYPE_DECOSTOP,
    DECO_TYPE_DEEPSTOP,
};

const char *translateDecoType(Napi::Env env, unsigned int type) 
{
    if (type < 0 || type >= DECO_TYPE_COUNT - 1)
    {
        char buff[128];
        snprintf(buff, 128, "Invalid deco sample type %u", type);
        throw Napi::TypeError::New(env, buff);
    }

    return deco_type_names[type];

}

const char *translateSampleEventType(Napi::Env env, unsigned int type)
{
    if (type < 0 || type >= EVENT_TYPE_COUNT - 1)
    {
        char buff[128];
        snprintf(buff, 128, "Invalid event sample type %u", type);
        throw Napi::TypeError::New(env, buff);
    }

    return event_type_names[type];
}

Napi::Object getAllSampleEventTypes(Napi::Env env)
{
    auto sampleTypes = Napi::Object::New(env);

    for (auto eventTypes : event_type_names)
    {
        sampleTypes.Set(eventTypes, eventTypes);
    }

    return sampleTypes;
}