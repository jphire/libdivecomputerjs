#pragma once
#include <libdivecomputer/context.h>
#include <napi.h>

#define LOGLEVEL_NONE "None"
#define LOGLEVEL_ERROR "Error"
#define LOGLEVEL_WARNING "Warning"
#define LOGLEVEL_INFO "Info"
#define LOGLEVEL_DEBUG "Debug"
#define LOGLEVEL_ALL "All"

dc_loglevel_t translateLogLevel(Napi::String);
Napi::String translateLogLevel(Napi::Env, dc_loglevel_t);
bool isLogLevel(Napi::String);

Napi::Object getLogLevels(Napi::Env);