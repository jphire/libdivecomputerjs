#pragma once
#include <napi.h>
#include <libdivecomputer/parser.h>

#define FIELD_ATMOSPHERIC "Atmospheric"
#define FIELD_AVGDEPTH "AverageDepth"
#define FIELD_DIVEMODE "DiveMode"
#define FIELD_DIVETIME "DiveTime"
#define FIELD_GASMIX "GasMix"
#define FIELD_GASMIX_COUNT "GasMixCount"
#define FIELD_MAXDEPTH "MaxDepth"
#define FIELD_SALINITY "Salinity"
#define FIELD_TANK "Tank"
#define FIELD_TANK_COUNT "TankCount"
#define FIELD_TEMPERATURE_MAXIMUM "TemperatureMaximum"
#define FIELD_TEMPERATURE_MINIMUM "TemperatureMinimum"
#define FIELD_TEMPERATURE_SURFACE "TemperatureSurface"

#define ALL_FIELDS                     \
    {                                  \
        FIELD_ATMOSPHERIC,             \
            FIELD_AVGDEPTH,            \
            FIELD_DIVEMODE,            \
            FIELD_DIVETIME,            \
            FIELD_GASMIX,              \
            FIELD_GASMIX_COUNT,        \
            FIELD_MAXDEPTH,            \
            FIELD_SALINITY,            \
            FIELD_TANK,                \
            FIELD_TANK_COUNT,          \
            FIELD_TEMPERATURE_MAXIMUM, \
            FIELD_TEMPERATURE_MINIMUM, \
            FIELD_TEMPERATURE_SURFACE, \
    }

Napi::Object getAllFieldTypes(Napi::Env env);
dc_field_type_t translateFieldType(Napi::Env env, Napi::String fieldtype);