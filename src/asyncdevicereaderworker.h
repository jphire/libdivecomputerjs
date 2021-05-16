#pragma once
#include <napi.h>
#include <libdivecomputer/device.h>
#include "transports/nativetransport.h"
#include "descriptor.h"
#include "context.h"
#include "values/divedata.h"
#include "values/logdata.h"
#include "values/eventdata.h"
#include "values/wrappeddata.h"
#include "contracts/errorable.h"
#include <chrono>
#include <thread>

struct fingerprint_t
{
    unsigned int size = 0;
    const unsigned char *data = NULL;
};

#define HANDLE_NAPI_ERROR(x, y)      \
    try                              \
    {                                \
        x                            \
    }                                \
    catch (const Napi::Error &error) \
    {                                \
        y                            \
    }

#define HANDLE_NAPI_WITH_ERRORABLE(x, errorable)      \
    HANDLE_NAPI_ERROR(x,                              \
                      {                               \
                          errorable->setError(error); \
                      })

class AsyncDeviceReaderWorker : public Errorable, public Napi::AsyncWorker
{
public:
    void setError(const Napi::Error &error) override;

    AsyncDeviceReaderWorker(Napi::Function &callback);
    ~AsyncDeviceReaderWorker();
    void setDescriptor(Descriptor *descriptor);
    void setContext(Context *context);
    void setTransport(const Napi::Object &transport);
    void setFingerprint(const unsigned char *fingerprint, unsigned int fsize);
    void setEventsCallback(unsigned int events, const Napi::Function &callback);
    void setDiveCallback(const Napi::Function &dive);
    void setDeviceCallback(const Napi::Function &dive);
    void cancel();

private:
    void Execute() override;
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
    volatile bool isCancelled = false;

    static int nativeForeachCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata);
    static void callWithDivedata(Napi::Env env, Napi::Function jsCallback, WrappedData<Errorable, Divedata> *data);
    static void callWithLogdata(Napi::Env env, Napi::Function jsCallback, WrappedData<Errorable, Logdata> *data);
    static void callWithEventdata(Napi::Env env, Napi::Function jsCallback, WrappedData<Errorable, Eventdata> *data);
    static void callWithDevicedata(Napi::Env env, Napi::Function jsCallback, WrappedData<Errorable, dc_device_t> *data);
};
