#include "parser.h"
#include "context.h"
#include "device.h"
#include "errors/DCError.h"
#include "enums/fieldtypes.h"
#include "enums/fieldvalues/divemode.h"
#include "enums/fieldvalues/tankvolume.h"
#include "enums/fieldvalues/watertype.h"

Parser::Parser(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<Parser>(info)
{

    if (info.Length() != 1 || !info[0].IsObject())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {device}.");
    }

    auto device = Napi::ObjectWrap<Device>::Unwrap(info[0].ToObject());
    auto status = dc_parser_new(&parser, device->getNative());
    DCError::AssertSuccess(info.Env(), status);
}

Parser::~Parser()
{
    if (parser)
    {
        dc_parser_destroy(parser);
        parser = NULL;
    }
}

void Parser::setData(const Napi::CallbackInfo &info)
{
    if (info.Length() == 1 && info[0].IsArrayBuffer())
    {
        throw Napi::TypeError::New(info.Env(), "Invalid arguments, expected {ArrayBuffer} given from device.foreach");
    }

    auto data = info[0].As<Napi::ArrayBuffer>();

    auto status = dc_parser_set_data(parser, reinterpret_cast<unsigned char *>(data.Data()), data.ByteLength());
    DCError::AssertSuccess(info.Env(), status);
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

Napi::Value Parser::getField(const Napi::CallbackInfo &info)
{
    if (info.Length() == 1 && info[0].IsString())
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
        DCError::AssertSuccess(env, status);

        return Napi::Number::New(env, numericValue);
    case DC_FIELD_DIVETIME:
    case DC_FIELD_GASMIX_COUNT:
    case DC_FIELD_TANK_COUNT:
        unsigned int integerValue;
        status = dc_parser_get_field(parser, field, 0, &integerValue);
        DCError::AssertSuccess(env, status);

        return Napi::Number::New(env, integerValue);
    case DC_FIELD_TANK:
        dc_tank_t tank;
        emptyTank(tank);

        status = dc_parser_get_field(parser, field, 0, &tank);
        DCError::AssertSuccess(env, status);

        return wrapTank(env, tank);
        break;
    case DC_FIELD_GASMIX:
        dc_gasmix_t gasmix;
        emptyGasmix(gasmix);

        status = dc_parser_get_field(parser, field, 0, &gasmix);
        DCError::AssertSuccess(env, status);

        return wrapGasmix(env, gasmix);
        break;
    case DC_FIELD_SALINITY:
        dc_salinity_t salinity;
        emptySalinity(salinity);

        status = dc_parser_get_field(parser, field, 0, &salinity);
        DCError::AssertSuccess(env, status);

        return wrapSalinity(env, salinity);
        break;
    case DC_FIELD_DIVEMODE:
        dc_divemode_t divemode;
        status = dc_parser_get_field(parser, field, 0, &divemode);
        DCError::AssertSuccess(env, status);

        return translateDiveMode(env, divemode);
    }

    throw Napi::TypeError::New(env, "Unsupported dc field type");
}
