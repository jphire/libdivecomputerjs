#pragma once
#include <libdivecomputer/iostream.h>

class NativeTransport
{
public:
    virtual dc_status_t getNative(dc_iostream_t **iostream, dc_context_t *ctx) = 0;
};