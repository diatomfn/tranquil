#include "string.h"

namespace JS {
    String::String(const char *string) {
        if (JsCreateString(string, strlen(string), &this->value) != JsNoError)
            throw FatalRuntimeException();
    }

    String::operator std::string() const {
        JsValueRef stringValue;

        if (JsConvertValueToString(this->value, &stringValue) != JsNoError)
            throw FatalRuntimeException();

        char* str = nullptr;
        int length;

        if (JsGetStringLength(stringValue, &length) != JsNoError)
            throw FatalRuntimeException();

        str = (char*) malloc(length + 1);

        if (JsCopyString(stringValue, str, length+1, nullptr) != JsNoError) {
            free(str);
            throw FatalRuntimeException();
        }

        str[length] = 0;

        std::string rVal = std::string(str);
        free(str);

        return rVal;
    }
}