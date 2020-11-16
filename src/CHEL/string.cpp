#include "string.h"

#include "common.h"

namespace JS::String {
    JsValueRef ToJS(const char* str) {
        JsValueRef js_str;

        if (JsCreateString(str, strlen(str), &js_str) != JsNoError)
            throw FatalRuntimeException();

        return js_str;
    }
    std::string FromJS(JsValueRef& value) {
        JsValueRef stringValue = ValueToStringJS(value);

        char* str = nullptr;
        int length;

        if (JsGetStringLength(stringValue, &length) != JsNoError)
            throw FatalRuntimeException();
        str = (char*) malloc(length + 1);

        if (JsCopyString(stringValue, str, length+1, nullptr) != JsNoError)
            throw FatalRuntimeException();

        str[length] = 0;

        std::string rVal = std::string(str);
        free(str);

        return rVal;
    }
    JsValueRef ValueToStringJS(JsValueRef& value) {
        JsValueRef stringValue;

        if (JsConvertValueToString(value, &stringValue) != JsNoError)
            throw FatalRuntimeException();

        return stringValue;
    }
}