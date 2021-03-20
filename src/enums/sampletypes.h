#pragma once
#include <napi.h>
#include <libdivecomputer/parser.h>

#define SAMPLE_TIME "Time"
#define SAMPLE_DEPTH "Depth"
#define SAMPLE_PRESSURE "Pressure"
#define SAMPLE_TEMPERATURE "Temperature"
#define SAMPLE_EVENT "Event"
#define SAMPLE_RBT "RBT"
#define SAMPLE_HEARTBEAT "Heartbeat"
#define SAMPLE_BEARING "Bearing"
#define SAMPLE_VENDOR "Vendor"
#define SAMPLE_SETPOINT "Setpoint"
#define SAMPLE_PPO2 "PPO2"
#define SAMPLE_CNS "CNS"
#define SAMPLE_DECO "Deco"
#define SAMPLE_GASMIX "Gasmix"

#define ALL_SAMPLE_TYPES        \
    {                           \
        SAMPLE_BEARING,         \
            SAMPLE_CNS,         \
            SAMPLE_DECO,        \
            SAMPLE_DEPTH,       \
            SAMPLE_EVENT,       \
            SAMPLE_GASMIX,      \
            SAMPLE_HEARTBEAT,   \
            SAMPLE_PPO2,        \
            SAMPLE_PRESSURE,    \
            SAMPLE_RBT,         \
            SAMPLE_SETPOINT,    \
            SAMPLE_TEMPERATURE, \
            SAMPLE_TIME,        \
            SAMPLE_VENDOR,      \
    }

Napi::Object getAllSampleTypes(Napi::Env);
Napi::String translateSampleType(Napi::Env, dc_sample_type_t sampleType);
dc_sample_type_t translateSampleType(Napi::Env, Napi::String sampleType);
