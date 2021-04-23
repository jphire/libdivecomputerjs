#pragma once
#include <cstdlib>
#include <libdivecomputer/device.h>
#include <napi.h>

class Eventdata
{
public:
    dc_event_type_t event;
    void *data = NULL;

    Eventdata(dc_event_type_t event, const void *data);
    ~Eventdata();
};

void *copyEventData(dc_event_type_t event, const void *data);
void *freeEventData(dc_event_type_t event, void *data);

Napi::Object wrapEvent(Napi::Env env, dc_event_type_t event, const void *data);