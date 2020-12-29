#include "array.h"

#include "CHEL/common.h"

namespace JS {
    JS::Value Array::operator[] (int index) {
        JsValueRef result;
        if (JsGetIndexedProperty(this->value, JS::Number(index), &result) != JsNoError)
            throw FatalRuntimeException();
        return JS::Value(result);
    }

    JS::Value Array::operator[] (JsValueRef index) {
        JsValueRef result;
        if (JsGetIndexedProperty(this->value, index, &result) != JsNoError)
            throw FatalRuntimeException();
        return JS::Value(result);
    }

    Array::Array(JsValueRef value) {
        this->value = value;
    }

    Array::Array() {
        if (JsCreateArray(0, &this->value) != JsNoError)
            throw FatalRuntimeException();
        this->SetupBindings();
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

    JS::Number Array::Length() {
        return JS::Number(this->length);
    }

    void Array::SetupBindings() {
        JS::Object obj(this->value);

        this->length = obj.GetProperty("length");
        this->push = obj.GetProperty("push");
        this->pop = obj.GetProperty("pop");
    }
}