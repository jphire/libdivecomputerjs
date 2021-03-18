#include <napi.h>
#include <libdivecomputer/parser.h>
#include <libdivecomputer/common.h>

#define WATER_FRESH "Fresh"
#define WATER_SALT "Salt"

#define ALL_WATER_TYPES  \
    {                    \
        WATER_SALT,      \
            WATER_FRESH, \
    }

Napi::Object getAllWaterTypes(Napi::Env);
dc_water_t translateWaterType(Napi::Env env, Napi::String salinity);
Napi::String translateWaterType(Napi::Env env, dc_water_t watertype);