#include "logdata.h"
#include <stdio.h>
#include <cstring>

Logdata::Logdata(dc_loglevel_t loglevel, const char *message)
    : loglevel(loglevel)
{
    auto len = strnlen(message, MAX_MESSAGE_LENGTH);
    this->message = (const char *)calloc(len, sizeof(const char *));
    memcpy((void *)this->message, message, len);
}

Logdata::~Logdata()
{
    if (message != NULL)
    {
        free((void *)message);
        message = NULL;
    }
}