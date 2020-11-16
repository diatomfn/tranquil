#include "number.h"

namespace JS::Number {
    JsValueRef ToJS(int num) {
        JsValueRef value;

        if (JsIntToNumber(num, &value) != JsNoError)
            throw FatalRuntimeException();

        return value;
    };

    int FromJS(JsValueRef value) {
        int num;

        if (JsNumberToInt(value, &num) != JsNoError)
            throw FatalRuntimeException();

        return num;
    };

    JsValueRef ValueToNumber(JsValueRef& value) {
        JsValueRef numberValue;

        if (JsConvertValueToNumber(value, &numberValue) != JsNoError)
            throw FatalRuntimeException();

        return numberValue;
    };
}