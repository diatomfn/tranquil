#include "boolean.h"

namespace JS {
    Boolean::Boolean(bool boolean) {
        if (JsBoolToBoolean(boolean, &this->value) != JsNoError)
            throw FatalRuntimeException();
    }
    bool Boolean::FromJS() {
        bool ret;
        if (JsBooleanToBool(this->value, &ret) != JsNoError)
            throw FatalRuntimeException();
        return ret;
    }
}