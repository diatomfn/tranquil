#include "boolean.h"

namespace JS {
    Boolean::Boolean(bool boolean) {
        if (JsBoolToBoolean(boolean, &this->value) != JsNoError)
            throw FatalRuntimeException();
    }

    Boolean::operator bool() const {
        bool ret;
        if (JsBooleanToBool(this->value, &ret) != JsNoError)
            throw FatalRuntimeException();
        return ret;
    }
}