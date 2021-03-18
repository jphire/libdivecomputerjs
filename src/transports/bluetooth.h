#pragma once
#include <napi.h>
#include <libdivecomputer/bluetooth.h>
#include "../context.h"
#include "../descriptor.h"

class BluetoothTransport : public Napi::ObjectWrap<BluetoothTransport>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Value iterate(const Napi::CallbackInfo &);

    BluetoothTransport(const Napi::CallbackInfo &);
    ~BluetoothTransport();

    Napi::Value getName(const Napi::CallbackInfo &);
    Napi::Value toString(const Napi::CallbackInfo &);
    Napi::Value open(const Napi::CallbackInfo &);

private:
    dc_bluetooth_device_t *device;
    static Napi::FunctionReference constructor;
};