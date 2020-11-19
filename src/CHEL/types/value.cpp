#include "value.h"

namespace JS {
    Value::operator JsValueRef() const {
        return this->value;
    }

    Value::Value() {
        if (JsGetUndefinedValue(&this->value) != JsNoError)
            throw FatalRuntimeException();
    }

    Value::Value(JsValueRef value) {
        this->value = value;
    }
}