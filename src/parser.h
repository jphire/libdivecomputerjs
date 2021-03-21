#include <napi.h>
#include <libdivecomputer/parser.h>
#define ReturnUndefinedOnUnsupported(status, env) \
    if (status == DC_STATUS_UNSUPPORTED)          \
    return env.Undefined()
#define GET_TIMEZONE(X) ((X == DC_TIMEZONE_NONE) ? 0 : X)

class Parser : public Napi::ObjectWrap<Parser>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);
    Parser(const Napi::CallbackInfo &);
    ~Parser();
    Napi::Value getDatetime(const Napi::CallbackInfo &);
    Napi::Value setData(const Napi::CallbackInfo &);
    Napi::Value getField(const Napi::CallbackInfo &);
    Napi::Value samplesForeach(const Napi::CallbackInfo &);

private:
    dc_parser_t *parser;
    Napi::FunctionReference sampleCallback;
    static void nativeSamplesCallback(dc_sample_type_t type, dc_sample_value_t value, void *userdata);
};