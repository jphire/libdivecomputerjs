#include <napi.h>
#include <libdivecomputer/device.h>
#include "context.h"
#include "descriptor.h"

class Device : public Napi::ObjectWrap<Device>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::FunctionReference constructor;

    Device(const Napi::CallbackInfo &);
    ~Device();
    void foreach (const Napi::CallbackInfo &);
    void setFingerprint(const Napi::CallbackInfo &);
    void setEvents(const Napi::CallbackInfo &);
    void setCancel(const Napi::CallbackInfo &);
    dc_device_t *getNative();

private:
    bool borrowed = false;

    Napi::FunctionReference eventCallback;
    Napi::FunctionReference cancelCallback;
    Napi::FunctionReference foreachCallback;
    Context *context;
    Descriptor *descriptor;
    dc_device_t *device;
    static void nativeEventCallback(dc_device_t *d, dc_event_type_t event, const void *data, void *userdata);
    static int nativeCancelCallback(void *userdata);
    static int nativeForeachCallback(const unsigned char *data, unsigned int size, const unsigned char *fingerprint, unsigned int fsize, void *userdata);
};
