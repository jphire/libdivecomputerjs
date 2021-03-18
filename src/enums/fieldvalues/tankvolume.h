#include <napi.h>
#include <libdivecomputer/parser.h>
#include <libdivecomputer/common.h>

#define TANKVOLUME_NONE "None"
#define TANKVOLUME_METRIC "Metric"
#define TANKVOLUME_IMPERIAL "Imperial"

#define ALL_TANKVOLUMES          \
    {                            \
        TANKVOLUME_NONE,         \
            TANKVOLUME_IMPERIAL, \
            TANKVOLUME_METRIC,   \
    }

Napi::Object getAllTankVolumes(Napi::Env);
dc_tankvolume_t translateTankVolume(Napi::Env env, Napi::String tankVolume);
Napi::String translateTankVolume(Napi::Env env, dc_tankvolume_t tankvolume);