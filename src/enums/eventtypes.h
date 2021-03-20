#pragma once
#include <napi.h>
#include <libdivecomputer/device.h>

#define EVENT_WAITING "Waiting"
#define EVENT_DEVINFO "DevInfo"
#define EVENT_PROGRESS "Progress"
#define EVENT_VENDOR "Vendor"
#define EVENT_CLOCK "Clock"

#define ALL_EVENT_TYPES     \
    {                       \
        EVENT_WAITING,      \
            EVENT_DEVINFO,  \
            EVENT_PROGRESS, \
            EVENT_VENDOR,   \
            EVENT_CLOCK,    \
    }

Napi::Object getAllEventTypes(Napi::Env);
unsigned int translateEvents(Napi::Env, Napi::Array);
dc_event_type_t translateEvent(Napi::Env, Napi::String);
const char *translateEvent(Napi::Env, dc_event_type_t event);