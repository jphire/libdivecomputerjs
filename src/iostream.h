#include <napi.h>
#include <libdivecomputer/iostream.h>

class IOStream : public Napi::ObjectWrap<IOStream>
{
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    IOStream(const Napi::CallbackInfo &);
    ~IOStream();
    dc_iostream_t *getNative();

private:
    dc_iostream_t *iostream;
};