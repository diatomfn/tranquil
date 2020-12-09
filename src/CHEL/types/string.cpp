#include "string.h"

namespace JS {
    String::String(const char *string) {
        if (JsCreateString(string, strlen(string), &this->value) != JsNoError)
            throw FatalRuntimeException();
    }
}