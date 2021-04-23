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

class AsyncDeviceReaderWorker : public Napi::AsyncWorker
{
public:
    AsyncDeviceReaderWorker(Napi::Function &callback);
    ~AsyncDeviceReaderWorker();
    void setDescriptor(Descriptor *descriptor);
    void setContext(Context *context);
    void setTransport(const Napi::Object &transport);
    void setEvents(unsigned int events, const Napi::Function &callback);
    void setDive(const Napi::Function &dive);

private:
    void
    Execute() override;
    void OnWorkComplete(Napi::Env env, napi_status status) override;

    Napi::ThreadSafeFunction tsfDivedata;
    Napi::ThreadSafeFunction tsfLogdata;
    Napi::ThreadSafeFunction tsfEventdata;
    Napi::FunctionReference diveCallback;
    Context *context = NULL;
    NativeTransport *transport = NULL;
    Descriptor *descriptor = NULL;
    Napi::ObjectReference transportRef;
    unsigned int events = 0;

    static int nativeForeachCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata);
    static void callWithDivedata(Napi::Env env, Napi::Function jsCallback, Divedata *data);
    static void callWithLogdata(Napi::Env env, Napi::Function jsCallback, Logdata *data);
    static void callWithEventdata(Napi::Env env, Napi::Function jsCallback, Eventdata *data);
};
