#include "fieldtypes.h"
#include <napi.h>
#include <libdivecomputer/parser.h>

Napi::Object getAllFieldTypes(Napi::Env env)
{
    auto fieldTypes = Napi::Object::New(env);
    auto allFields = ALL_FIELDS;

    for (auto field : allFields)
    {
        fieldTypes.Set(field, field);
    }

    return fieldTypes;
}

dc_field_type_t translateFieldType(Napi::Env env, Napi::String fieldtype)
{
    auto fieldtypeString = fieldtype.Utf8Value();
    if (fieldtypeString == FIELD_ATMOSPHERIC)
    {
        return DC_FIELD_ATMOSPHERIC;
    }

    if (fieldtypeString == FIELD_AVGDEPTH)
    {
        return DC_FIELD_AVGDEPTH;
    }

    if (fieldtypeString == FIELD_DIVEMODE)
    {
        return DC_FIELD_DIVEMODE;
    }

    if (fieldtypeString == FIELD_DIVETIME)
    {
        return DC_FIELD_DIVETIME;
    }

    if (fieldtypeString == FIELD_GASMIX)
    {
        return DC_FIELD_GASMIX;
    }

    if (fieldtypeString == FIELD_GASMIX_COUNT)
    {
        return DC_FIELD_GASMIX_COUNT;
    }

    if (fieldtypeString == FIELD_MAXDEPTH)
    {
        return DC_FIELD_MAXDEPTH;
    }

    if (fieldtypeString == FIELD_SALINITY)
    {
        return DC_FIELD_SALINITY;
    }

    if (fieldtypeString == FIELD_TANK)
    {
        return DC_FIELD_TANK;
    }

    if (fieldtypeString == FIELD_TANK_COUNT)
    {
        return DC_FIELD_TANK_COUNT;
    }

    if (fieldtypeString == FIELD_TEMPERATURE_MAXIMUM)
    {
        return DC_FIELD_TEMPERATURE_MAXIMUM;
    }

    if (fieldtypeString == FIELD_TEMPERATURE_MINIMUM)
    {
        return DC_FIELD_TEMPERATURE_MINIMUM;
    }

    if (fieldtypeString == FIELD_TEMPERATURE_SURFACE)
    {
        return DC_FIELD_TEMPERATURE_SURFACE;
    }

    char buffer[128];
    snprintf(buffer, 128, "Invalid Field type provided: %s", fieldtypeString.c_str());
    throw Napi::TypeError::New(env, buffer);
}