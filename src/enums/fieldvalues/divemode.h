#pragma once
#include <napi.h>
#include <libdivecomputer/parser.h>
#include <libdivecomputer/common.h>

#define DIVEMODE_FREEDIVE "Freedive"
#define DIVEMODE_GAUGE "Gauge"
#define DIVEMODE_OC "OpenCircuit"
#define DIVEMODE_CCR "ClosedCircuitRebreather"
#define DIVEMODE_SCR "SemiclosedCircuitRebreather"

#define ALL_DIVEMODES       \
    {                       \
        DIVEMODE_FREEDIVE,  \
            DIVEMODE_GAUGE, \
            DIVEMODE_OC,    \
            DIVEMODE_CCR,   \
            DIVEMODE_SCR,   \
    }

Napi::Object getAllDiveModes(Napi::Env);
dc_divemode_t translateDiveMode(Napi::Env env, Napi::String divemode);
Napi::String translateDiveMode(Napi::Env env, dc_divemode_t divemode);