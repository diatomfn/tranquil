#include "boolean.h"

namespace JS::Boolean {
    bool FromJS(JsValueRef value) {
        bool ret;
        if (JsBooleanToBool(value, &ret) != JsNoError)
            throw FatalRuntimeException();
        return ret;
    }

    JsValueRef ToJS(bool value) {
        JsValueRef ret;
        if (JsBoolToBoolean(value, &ret) != JsNoError)
            throw FatalRuntimeException();
        return ret;
    }
}