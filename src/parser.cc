#include "parser.h"
#include "context.h"
#include "device.h"
#include "descriptor.h"
#include "errors/DCError.h"
#include "enums/fieldtypes.h"
#include "enums/sampletypes.h"
#include "enums/fieldvalues/divemode.h"
#include "enums/fieldvalues/tankvolume.h"
#include "enums/fieldvalues/watertype.h"
#include "enums/samplevalues/eventsampletype.h"

Napi::FunctionReference Parser::constructor;

void Parser::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "Parser",
        {
            InstanceMethod<&Parser::setData>("setData"),
            InstanceMethod<&Parser::setData>("setData"),
            InstanceMethod<&Parser::getField>("getField"),
            InstanceMethod<&Parser::getDatetime>("getDatetime"),
            InstanceMethod<&Parser::samplesForeach>("samplesForeach"),
        });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Parser", func);
}

dc_parser_t *Parser::fromDevice(const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsObject())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {device}.");
    }

    dc_parser_t *parser;
    auto device = Napi::ObjectWrap<Device>::Unwrap(info[0].ToObject());
    auto status = dc_parser_new(&parser, device->getNative());
    DCError::AssertSuccess(info.Env(), status);

    return parser;
}

dc_parser_t *Parser::fromData(const Napi::CallbackInfo &info)
{
    if (info.Length() != 4 || !info[0].IsObject() || !info[1].IsObject() || !info[2].IsNumber() || !info[3].IsBigInt())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {context}, {descriptor}, {devtime: number}, {systime: number}");
    }

    auto context = Napi::ObjectWrap<Context>::Unwrap(info[0].ToObject());
    auto descriptor = Napi::ObjectWrap<Descriptor>::Unwrap(info[1].ToObject());
    auto devtime = info[2].ToNumber().Uint32Value();
    bool lossless = false;
    auto systime = info[3].As<Napi::BigInt>().Int64Value(&lossless);

    dc_parser_t *parser;
    auto status = dc_parser_new2(&parser, context->getNative(), descriptor->getNative(), devtime, systime);
    DCError::AssertSuccess(info.Env(), status);

    return parser;
}

Parser::Parser(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<Parser>(info)
{

    if (info.Length() == 1)
    {
        parser = Parser::fromDevice(info);
    }
    else if (info.Length() == 4)
    {
        parser = Parser::fromData(info);
    }
    else
    {
        throw Napi::TypeError::New(info.Env(), "Unable to construct parser, use `new Parser({device})` or new Parser({context}, {descriptor}, {devtime: number}, {systime: number})");
    }
}

Parser::~Parser()
{
    if (parser)
    {
        dc_parser_destroy(parser);
        parser = NULL;
    }
}

Napi::Value Parser::setData(const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsBuffer())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {ArrayBuffer} given from device.foreach");
    }

    auto data = info[0].As<Napi::Buffer<unsigned char>>();

    auto status = dc_parser_set_data(parser, data.Data(), data.ByteLength());
    DCError::AssertSuccess(info.Env(), status);

    return info.Env().Undefined();
}

Napi::Object wrapTank(Napi::Env env, dc_tank_t &tank)
{
    auto tankObject = Napi::Object::New(env);

    tankObject.Set("type", translateTankVolume(env, tank.type));
    tankObject.Set("beginPressure", tank.beginpressure);
    tankObject.Set("endPressure", tank.endpressure);
    tankObject.Set("workingPressure", tank.workpressure);
    tankObject.Set("volume", tank.volume);
    tankObject.Set("gasmix", tank.gasmix);

    return tankObject;
}

Napi::Object wrapSalinity(Napi::Env env, dc_salinity_t &salinity)
{
    auto salinityObject = Napi::Object::New(env);

    salinityObject.Set("density", salinity.density);
    salinityObject.Set("type", translateWaterType(env, salinity.type));

    return salinityObject;
}

Napi::Object wrapGasmix(Napi::Env env, dc_gasmix_t &gasmix)
{
    auto gasmixObject = Napi::Object::New(env);

    gasmixObject.Set("helium", gasmix.helium);
    gasmixObject.Set("nitrogen", gasmix.nitrogen);
    gasmixObject.Set("oxygen", gasmix.oxygen);

    return gasmixObject;
}

inline void emptyTank(dc_tank_t &tank)
{
    tank.beginpressure = 0;
    tank.endpressure = 0;
    tank.gasmix = 0;
    tank.type = DC_TANKVOLUME_NONE;
    tank.volume = 0;
    tank.workpressure = 0;
}

inline void emptyGasmix(dc_gasmix_t &gasmix)
{
    gasmix.oxygen = 0;
    gasmix.nitrogen = 0;
    gasmix.helium = 0;
}

inline void emptySalinity(dc_salinity_t &salinity)
{
    salinity.density = 0;
    salinity.type = DC_WATER_FRESH;
}

Napi::Value Parser::getDatetime(const Napi::CallbackInfo &info)
{
    dc_datetime_t dt;
    auto status = dc_parser_get_datetime(parser, &dt);
    DCError::AssertSuccess(info.Env(), status);

    char buff[26];
    snprintf(
        buff,
        26,
        "%04d-%02d-%02dT%02d:%02d:%02d+%02d:%02d",
        dt.year,
        dt.month,
        dt.day,
        dt.hour,
        dt.minute,
        dt.second,
        GET_TIMEZONE(dt.timezone) / 3600,
        GET_TIMEZONE(dt.timezone) % 3600);
    return Napi::String::New(info.Env(), buff);
}

Napi::Value Parser::getField(const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsString())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {string} where string is any of FieldTypes");
    }

    auto env = info.Env();
    auto field = translateFieldType(info.Env(), info[0].ToString());
    dc_status_t status;

    switch (field)
    {
    case DC_FIELD_ATMOSPHERIC:
    case DC_FIELD_AVGDEPTH:
    case DC_FIELD_MAXDEPTH:
    case DC_FIELD_TEMPERATURE_SURFACE:
    case DC_FIELD_TEMPERATURE_MAXIMUM:
    case DC_FIELD_TEMPERATURE_MINIMUM:
        double numericValue;
        status = dc_parser_get_field(parser, field, 0, &numericValue);
        ReturnUndefinedOnUnsupported(status, info.Env());
        DCError::AssertSuccess(env, status);

        return Napi::Number::New(env, numericValue);
    case DC_FIELD_DIVETIME:
    case DC_FIELD_GASMIX_COUNT:
    case DC_FIELD_TANK_COUNT:
        unsigned int integerValue;
        status = dc_parser_get_field(parser, field, 0, &integerValue);
        ReturnUndefinedOnUnsupported(status, info.Env());
        DCError::AssertSuccess(env, status);

        return Napi::Number::New(env, integerValue);
    case DC_FIELD_TANK:
        dc_tank_t tank;
        emptyTank(tank);

        status = dc_parser_get_field(parser, field, 0, &tank);
        ReturnUndefinedOnUnsupported(status, info.Env());
        DCError::AssertSuccess(env, status);

        return wrapTank(env, tank);
        break;
    case DC_FIELD_GASMIX:
        dc_gasmix_t gasmix;
        emptyGasmix(gasmix);

        status = dc_parser_get_field(parser, field, 0, &gasmix);
        ReturnUndefinedOnUnsupported(status, info.Env());
        DCError::AssertSuccess(env, status);

        return wrapGasmix(env, gasmix);
        break;
    case DC_FIELD_SALINITY:
        dc_salinity_t salinity;
        emptySalinity(salinity);

        status = dc_parser_get_field(parser, field, 0, &salinity);
        ReturnUndefinedOnUnsupported(status, info.Env());
        DCError::AssertSuccess(env, status);

        return wrapSalinity(env, salinity);
        break;
    case DC_FIELD_DIVEMODE:
        dc_divemode_t divemode;
        status = dc_parser_get_field(parser, field, 0, &divemode);
        ReturnUndefinedOnUnsupported(status, info.Env());
        DCError::AssertSuccess(env, status);

        return translateDiveMode(env, divemode);
    }

    throw Napi::TypeError::New(env, "Unsupported dc field type");
}

Napi::Object wrapDecoSample(Napi::Env env, dc_sample_value_t value)
{
    auto deco = Napi::Object::New(env);
    deco.Set("depth", value.deco.depth);
    deco.Set("time", value.deco.time);
    deco.Set("type", value.deco.type);
    return deco;
}

Napi::Object wrapEventSample(Napi::Env env, dc_sample_value_t value)
{
    auto event = Napi::Object::New(env);
    event.Set("time", value.event.time);
    event.Set("flags", value.event.flags);
    event.Set("type", translateSampleEventType(env, value.event.type));
    event.Set("value", value.event.value);
    return event;
}

Napi::Object wrapPressureSample(Napi::Env env, dc_sample_value_t value)
{
    auto pressure = Napi::Object::New(env);
    pressure.Set("tank", value.pressure.tank);
    pressure.Set("value", value.pressure.value);
    return pressure;
}

Napi::Object wrapVendorSample(Napi::Env env, dc_sample_value_t value)
{
    auto vendor = Napi::Object::New(env);
    vendor.Set("type", value.vendor.type);
    vendor.Set("data", Napi::ArrayBuffer::New(env, (void *)value.vendor.data, value.vendor.size));
    return vendor;
}

Napi::Value getSampleValue(Napi::Env env, dc_sample_type_t type, dc_sample_value_t value)
{
    switch (type)
    {
    case DC_SAMPLE_BEARING:
        return Napi::Number::New(env, value.bearing);
    case DC_SAMPLE_CNS:
        return Napi::Number::New(env, value.cns);
    case DC_SAMPLE_DECO:
        return wrapDecoSample(env, value);
    case DC_SAMPLE_DEPTH:
        return Napi::Number::New(env, value.depth);
    case DC_SAMPLE_EVENT:
        return wrapEventSample(env, value);
    case DC_SAMPLE_GASMIX:
        return Napi::Number::New(env, value.gasmix);
    case DC_SAMPLE_HEARTBEAT:
        return Napi::Number::New(env, value.heartbeat);
    case DC_SAMPLE_PPO2:
        return Napi::Number::New(env, value.ppo2);
    case DC_SAMPLE_PRESSURE:
        return wrapPressureSample(env, value);
    case DC_SAMPLE_RBT:
        return Napi::Number::New(env, value.rbt);
    case DC_SAMPLE_SETPOINT:
        return Napi::Number::New(env, value.setpoint);
    case DC_SAMPLE_TEMPERATURE:
        return Napi::Number::New(env, value.temperature);
    case DC_SAMPLE_TIME:
        return Napi::Number::New(env, value.time);
    case DC_SAMPLE_VENDOR:
        return wrapVendorSample(env, value);
    }

    throw Napi::TypeError::New(env, "Unsupported dc sample ");
}

Napi::Object wrapSample(Napi::Env env, dc_sample_type_t type, dc_sample_value_t value)
{
    auto obj = Napi::Object::New(env);

    obj.Set("type", translateSampleType(env, type));
    obj.Set("value", getSampleValue(env, type, value));

    return obj;
}

void Parser::nativeSamplesCallback(dc_sample_type_t type, dc_sample_value_t value, void *userdata)
{
    auto parser = (Parser *)userdata;

    parser->sampleCallback.Call({
        wrapSample(parser->Env(), type, value),
    });
}

Napi::Value Parser::samplesForeach(const Napi::CallbackInfo &info)
{
    if (info.Length() != 1 || !info[0].IsFunction())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {function}");
    }

    sampleCallback = Napi::Persistent(info[0].As<Napi::Function>());

    dc_parser_samples_foreach(parser, &nativeSamplesCallback, this);

    return info.Env().Undefined();
}