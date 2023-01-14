#include "oar/Date.h"
#include <time.h>

namespace oar {
Date::Date(time_t t)
{
    struct tm ts;
    gmtime_r(&t, &ts);
    _ymd._day = ts.
}
}
