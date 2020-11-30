#include "array.h"

#include "CHEL/common.h"

namespace JS {
    Array::Array(JsValueRef value) {
        this->value = value;
        this->GetBindings();
    }

    Array::Array() {
        if (JsCreateArray(0, &this->value) != JsNoError)
            throw FatalRuntimeException();
        this->GetBindings();
    }

    JS::Value Array::Push(JsValueRef value) {
        JsValueRef arguments[] = {this->value, value};

        JsValueRef result;

        if (JsCallFunction(this->push, arguments, 2, &result) != JsNoError)
            throw FatalRuntimeException();

        return JS::Value(this->length);
    }

    JS::Value Array::Pop() {
        JsValueRef arguments[] = {this->value};

        JsValueRef result;

        if (JsCallFunction(this->pop, arguments, 1, &result) != JsNoError)
            throw FatalRuntimeException();

        return JS::Value(result);
    }

    void Array::GetBindings() {
        JS::Object obj(this->value);

        this->length = obj.GetProperty("length");
        this->push = obj.GetProperty("push");
        this->pop = obj.GetProperty("pop");
    }
}