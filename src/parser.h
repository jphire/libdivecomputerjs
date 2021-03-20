#include <napi.h>
#include <libdivecomputer/parser.h>

class Parser : public Napi::ObjectWrap<Parser>
{
public:
    static void Init(Napi::Env env, Napi::Object exports);
    Parser(const Napi::CallbackInfo &);
    ~Parser();
    Napi::Value setData(const Napi::CallbackInfo &);
    Napi::Value getField(const Napi::CallbackInfo &);
    Napi::Value samplesForeach(const Napi::CallbackInfo &);

private:
    dc_parser_t *parser;
    Napi::FunctionReference sampleCallback;
    static void nativeSamplesCallback(dc_sample_type_t type, dc_sample_value_t value, void *userdata);
};