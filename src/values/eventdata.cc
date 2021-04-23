#include "eventdata.h"
#include "../enums/eventtypes.h"
#include <napi.h>

Eventdata::Eventdata(dc_event_type_t event, const void *data)
    : event(event), data(copyEventData(event, data))
{
}

Eventdata::~Eventdata()
{
    if (data != NULL)
    {
        free(data);
        data = NULL;
    }
}

Napi::Value wrapClockEventData(Napi::Env env, const dc_event_clock_t *clock)
{
    auto clockObject = Napi::Object::New(env);

    clockObject.Set("devtime", clock->devtime);
    clockObject.Set("systime", clock->systime);

    return clockObject;
}

Napi::Value wrapProgressEventData(Napi::Env env, const dc_event_progress_t *progress)
{
    auto progressObject = Napi::Object::New(env);

    progressObject.Set("current", progress->current);
    progressObject.Set("maximum", progress->maximum);

    return progressObject;
}

Napi::Value wrapVendorEventData(Napi::Env env, const dc_event_vendor_t *vendor)
{
    return Napi::ArrayBuffer::New(env, const_cast<unsigned char *>(vendor->data), vendor->size);
}

Napi::Value wrapDevInfoEventData(Napi::Env env, const dc_event_devinfo_t *devinfo)
{
    auto progressObject = Napi::Object::New(env);

    progressObject.Set("firmware", devinfo->firmware);
    progressObject.Set("model", devinfo->model);
    progressObject.Set("serial", devinfo->serial);

    return progressObject;
}

Napi::Value wrapEventData(Napi::Env env, dc_event_type_t event, const void *data)
{
    auto eventObject = Napi::Object::New(env);
    switch (event)
    {
    case DC_EVENT_CLOCK:
        return wrapClockEventData(env, reinterpret_cast<const dc_event_clock_t *>(data));

    case DC_EVENT_PROGRESS:
        return wrapProgressEventData(env, reinterpret_cast<const dc_event_progress_t *>(data));

    case DC_EVENT_WAITING:
        return env.Undefined();

    case DC_EVENT_VENDOR:
        return wrapVendorEventData(env, reinterpret_cast<const dc_event_vendor_t *>(data));

    case DC_EVENT_DEVINFO:
        return wrapDevInfoEventData(env, reinterpret_cast<const dc_event_devinfo_t *>(data));
    }

    char buffer[128];
    sprintf(buffer, "Invalid event type %u", event);
    throw Napi::TypeError::New(env, buffer);
}

Napi::Object wrapEvent(Napi::Env env, dc_event_type_t event, const void *data)
{
    auto eventObject = Napi::Object::New(env);
    eventObject.Set("type", translateEvent(env, event));
    eventObject.Set("data", wrapEventData(env, event, data));
    return eventObject;
}

// size_t sizeofEventData(dc_event_type_t event)
// {
//     switch (event)
//     {
//     case DC_EVENT_CLOCK:
//         return sizeof(dc_event_clock_t);
//     case DC_EVENT_DEVINFO:
//         return sizeof(dc_event_devinfo_t);
//     case DC_EVENT_PROGRESS:
//         return sizeof(dc_event_progress_t);
//     }

//     return 0;
// }

void *copyEventData(dc_event_type_t event, const void *data)
{
    auto size = sizeof(data);
    auto copiedData = malloc(size);
    memcpy(copiedData, data, size);

    return copiedData;
}