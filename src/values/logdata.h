#pragma once
#include <cstdlib>
#include <libdivecomputer/context.h>

#define MAX_MESSAGE_LENGTH 256

class Logdata
{
public:
    const char *message = NULL;
    dc_loglevel_t loglevel;
    Logdata(dc_loglevel_t loglevel, const char *message);
    ~Logdata();
};
