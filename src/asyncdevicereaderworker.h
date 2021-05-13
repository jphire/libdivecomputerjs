#pragma once
#include <napi.h>
#include <libdivecomputer/device.h>
#include "transports/nativetransport.h"
#include "descriptor.h"
#include "context.h"
#include "values/divedata.h"
#include "values/logdata.h"
#include "values/eventdata.h"
#include <chrono>
#include <thread>

struct fingerprint_t
{
    unsigned int size = 0;
    const unsigned char *data = NULL;
};

class AsyncDeviceReaderWorker : public Napi::AsyncWorker
{
public:
    AsyncDeviceReaderWorker(Napi::Function &callback);
    ~AsyncDeviceReaderWorker();
    void setDescriptor(Descriptor *descriptor);
    void setContext(Context *context);
    void setTransport(const Napi::Object &transport);
    void setFingerprint(const unsigned char *fingerprint, unsigned int fsize);
    void setEventsCallback(unsigned int events, const Napi::Function &callback);
    void setDiveCallback(const Napi::Function &dive);
    void setDeviceCallback(const Napi::Function &dive);

private:
    void
    Execute() override;
    void OnWorkComplete(Napi::Env env, napi_status status) override;

    Napi::ThreadSafeFunction tsfDivedata;
    Napi::ThreadSafeFunction tsfLogdata;
    Napi::ThreadSafeFunction tsfEventdata;
    Napi::ThreadSafeFunction tsfDevicedata;

    Context *context = NULL;
    NativeTransport *transport = NULL;
    Descriptor *descriptor = NULL;
    dc_device_t *device = NULL;
    Napi::ObjectReference transportRef;
    unsigned int events = 0;
    fingerprint_t fingerprint;

    static int nativeForeachCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata);
    static void callWithDivedata(Napi::Env env, Napi::Function jsCallback, Divedata *data);
    static void callWithLogdata(Napi::Env env, Napi::Function jsCallback, Logdata *data);
    static void callWithEventdata(Napi::Env env, Napi::Function jsCallback, Eventdata *data);
    static void callWithDevicedata(Napi::Env env, Napi::Function jsCallback, dc_device_t *data);
};
