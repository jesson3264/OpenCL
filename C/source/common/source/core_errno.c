#include "core_errno.h"
#include "scl_cpystrn.h"

static char *stuffbuffer(char *buf, size_t bufsize, const char *s)
{
    //scl_cpystrn(buf, s, bufsize);
    return buf;
}

static char *apr_error_string(scl_status_t statcode)
{
    return NULL;
}

static char *native_strerror(scl_status_t statcode, char *buf,
                             size_t bufsize)
{

    const char *err = strerror(statcode);
    if (err) 
    {
        return stuffbuffer(buf, bufsize, err);
    } else 
    {
        return stuffbuffer(buf, bufsize, 
                           "SCL does not understand this error code");
    }
}

char* scl_strerror(scl_status_t statcode, char *buf, size_t bufsize)
{
    if (statcode < SCL_OS_START_ERROR)
    {
        return native_strerror(statcode, buf, bufsize);
    }
    else if (statcode < SCL_OS_START_USERERR)
    {

    }

    return NULL;
}
