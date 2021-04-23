#include <napi.h>
#include "asyncdevicereaderworker.h"

class AsyncDeviceReader : public Napi::ObjectWrap<AsyncDeviceReader>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);

    AsyncDeviceReader(const Napi::CallbackInfo &info);
    void setDescriptor(const Napi::CallbackInfo &info);
    void setContext(const Napi::CallbackInfo &info);
    void setTransport(const Napi::CallbackInfo &info);
    void setEventsCallback(const Napi::CallbackInfo &info);
    void setDiveCallback(const Napi::CallbackInfo &info);
    void read(const Napi::CallbackInfo &info);

private:
    Descriptor *descriptor = NULL;
    Context *context = NULL;
    Napi::ObjectReference transport;
    Napi::FunctionReference diveCallback;
    unsigned int events = 0;
    Napi::FunctionReference eventCallback;
};
