#pragma once
#include <napi.h>
#include <libdivecomputer/common.h>

#define TRANSPORT_USBHID "USBHID"
#define TRANSPORT_BLUETOOTH "Bluetooth"
#define TRANSPORT_IRDA "IRDA"
#define TRANSPORT_SERIAL "Serial"
#define TRANSPORT_USB "USB"
#define TRANSPORT_BLE "BLE"
#define TRANSPORT_NONE "None"

#define ALL_TRANSPORTS           \
    {                            \
        TRANSPORT_NONE,          \
            TRANSPORT_SERIAL,    \
            TRANSPORT_USBHID,    \
            TRANSPORT_IRDA,      \
            TRANSPORT_BLUETOOTH, \
            TRANSPORT_USB,       \
            TRANSPORT_BLE,       \
    }

Napi::Object getAllTransports(Napi::Env);
Napi::Array translateTransports(Napi::Env, unsigned int transports);