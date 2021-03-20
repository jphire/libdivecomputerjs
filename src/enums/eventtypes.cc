#include "eventtypes.h"
#include <libdivecomputer/device.h>

Napi::Object getAllEventTypes(Napi::Env env)
{
    auto values = ALL_EVENT_TYPES;
    auto allEventTypes = Napi::Object::New(env);

    for (auto value : values)
    {
        allEventTypes.Set(
            Napi::String::New(env, value),
            Napi::String::New(env, value));
    }

    return allEventTypes;
}

unsigned int translateEvents(Napi::Env env, Napi::Array array)
{
    unsigned int eventTypes = 0;

    for (unsigned int iX = 0, iMax = array.Length(); iX < iMax; iX++)
    {
        auto eventType = array.Get(iX);
        eventTypes |= translateEvent(env, eventType.ToString());
    }

    return eventTypes;
}

dc_event_type_t translateEvent(Napi::Env env, Napi::String value)
{
    auto eventString = value.Utf8Value().c_str();

    printf("'%s' == '%s'", EVENT_CLOCK, eventString);
    printf("'%u' == '%u'", strlen(EVENT_CLOCK), strlen(eventString));
    if (eventString == EVENT_CLOCK)
    {
        return DC_EVENT_CLOCK;
    }
    if (eventString == EVENT_WAITING)
    {
        return DC_EVENT_WAITING;
    }
    if (eventString == EVENT_VENDOR)
    {
        return DC_EVENT_VENDOR;
    }
    if (eventString == EVENT_PROGRESS)
    {
        return DC_EVENT_PROGRESS;
    }
    if (eventString == EVENT_DEVINFO)
    {
        return DC_EVENT_DEVINFO;
    }

    char buffer[128];
    sprintf(buffer, "Invalid event type '%s'", eventString);
    throw Napi::TypeError::New(env, buffer);
}

const char *translateEvent(Napi::Env env, dc_event_type_t event)
{
    switch (event)
    {
    case DC_EVENT_CLOCK:
        return EVENT_CLOCK;
    case DC_EVENT_DEVINFO:
        return EVENT_DEVINFO;
    case DC_EVENT_PROGRESS:
        return EVENT_PROGRESS;
    case DC_EVENT_VENDOR:
        return EVENT_VENDOR;
    case DC_EVENT_WAITING:
        return EVENT_WAITING;
    }

    char buffer[128];
    sprintf(buffer, "Invalid event type %u", event);
    throw Napi::TypeError::New(env, buffer);
}