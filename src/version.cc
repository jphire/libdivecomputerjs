#include "version.h"
#include <napi.h>
#include <libdivecomputer/version.h>
#include <stdio.h>

using namespace Napi;

Napi::String getVersion(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    dc_version_t ver;
    dc_version(&ver);
    char output[16];
    snprintf(output, 16, "%d.%d.%d", ver.major, ver.minor, ver.micro);

    return Napi::String::New(env, output);
}