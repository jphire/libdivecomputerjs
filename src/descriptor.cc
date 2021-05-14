#include "descriptor.h"
#include "enums/transport.h"
#include "errors/DCError.h"
#include <libdivecomputer/iterator.h>
#include <libdivecomputer/context.h>

using namespace Napi;

Napi::FunctionReference Descriptor::constructor;

Descriptor::Descriptor(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<Descriptor>(info)
{
    if (info.Length() != 1 || !info[0].IsExternal())
    {
        throw Napi::TypeError::New(info.Env(), "Cannot construct Descriptor from JS. Use Descriptor.iterate() to iterate all available.");
    }

    dc_descriptor_t *descriptor = info[0].As<Napi::External<dc_descriptor_t>>().Data();

    this->descriptor = descriptor;
}

Descriptor::~Descriptor()
{
    if (descriptor)
    {
        dc_descriptor_free(descriptor);
        descriptor = NULL;
    }
}

Napi::Object Descriptor::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env,
        "Descriptor",
        {
            InstanceAccessor<&Descriptor::getProduct>("product"),
            InstanceAccessor<&Descriptor::getVendor>("vendor"),
            InstanceAccessor<&Descriptor::getModel>("model"),
            InstanceAccessor<&Descriptor::getType>("type"),
            InstanceAccessor<&Descriptor::getTransports>("transports"),
            StaticMethod<&Descriptor::iterate>("iterate"),
        });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Descriptor", func);

    return exports;
}

Napi::Value Descriptor::getVendor(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), dc_descriptor_get_vendor(descriptor));
}

Napi::Value Descriptor::getProduct(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), dc_descriptor_get_product(descriptor));
}

Napi::Value Descriptor::getModel(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), dc_descriptor_get_model(descriptor));
}

Napi::Value Descriptor::getTransports(const Napi::CallbackInfo &info)
{
    return translateTransports(info.Env(), dc_descriptor_get_transports(descriptor));
}

Napi::Value Descriptor::getType(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), dc_descriptor_get_type(descriptor));
}

Napi::Object wrapDescriptor(Napi::Env env, dc_descriptor_t *descriptor)
{
    return Descriptor::constructor.New({Napi::External<dc_descriptor_t>::New(env, descriptor)});
}

dc_descriptor_t *Descriptor::getNative()
{
    return descriptor;
}

Napi::Value Descriptor::iterate(const Napi::CallbackInfo &info)
{
    dc_status_t status;
    auto env = info.Env();
    auto array = Napi::Array::New(env);

    dc_iterator_t *iterator;
    dc_descriptor_t *descriptor;

    status = dc_descriptor_iterator(&iterator);
    DCError::AssertSuccess(env, status);

    int i = 0;
    while ((status = dc_iterator_next(iterator, &descriptor)) != DC_STATUS_DONE)
    {
        DCError::AssertSuccess(env, status);

        array[i++] = wrapDescriptor(env, descriptor);
    }

    dc_iterator_free(iterator);

    return array;
}
