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
        JsValueRef value;

        if (JsConvertValueToString(this->value, &value) != JsNoError)
            throw FatalRuntimeException();
 
        char* str = nullptr;
        size_t length;

        // get the length of the value
        if (JsCopyString(value, nullptr, 0, &length) != JsNoError)
            throw FatalRuntimeException();

        str = (char*) malloc(length + 1);

        // get the value as a string
        if (JsCopyString(value, str, length+1, nullptr) != JsNoError) {
            free(str);
            throw FatalRuntimeException();
        }

        str[length] = 0;

        std::string rVal(str);

        free(str);

        return rVal;
    }

    JsValueType Value::GetType() {
        return JS::Value::GetType(this->value);
    }

    JsValueType Value::GetType(JsValueRef value) {
        JsValueType type;
        if (JsGetValueType(value, &type) != JsNoError)
            throw FatalRuntimeException();
        return type;
    }

    const char* Value::GetTypeString(JsValueType type) {
        switch (type) {
            case JsString:
                return "string";
            case JsNumber:
                return "number";
            case JsFunction:
                return "function";
            case JsNull:
                return "null";
            case JsUndefined:
                return "undefined";
            case JsError:
                return "error";
            case JsObject:
                return "object";
            case JsArray:
                return "array";
            case JsArrayBuffer:
                return "arraybuffer";
            case JsTypedArray:
                return "typedarray";
            case JsBoolean:
                return "boolean";
            case JsDataView:
                return "dataview";
            case JsSymbol:
                return "symbol";
            default:
                return "value";
        }
    }

    const char* Value::GetTypeString(JsValueRef value) {
        return JS::Value::GetTypeString(JS::Value::GetType(value));
    }

    const char* Value::GetTypeString() {
        return JS::Value::GetTypeString(this->value);
    }
}