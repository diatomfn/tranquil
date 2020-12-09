#include "value.h"

namespace JS {
    Value::operator JsValueRef () const {
        return this->value;
    }

    Value::Value() {
        if (JsGetUndefinedValue(&this->value) != JsNoError)
            throw FatalRuntimeException();
    }

    Value::Value(JsValueRef value) {
        this->value = value;
    }

    Value::operator std::string() const {
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

        std::string rVal(str);

        free(str);

        return rVal;
    }
}