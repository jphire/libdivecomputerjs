#include <napi.h>
#include <libdivecomputer/parser.h>

class Parser : public Napi::ObjectWrap<Parser>
{
public:
    Parser(const Napi::CallbackInfo &);
    ~Parser();
    void setData(const Napi::CallbackInfo &);
    Napi::Value getField(const Napi::CallbackInfo &);

private:
    dc_parser_t *parser;
};