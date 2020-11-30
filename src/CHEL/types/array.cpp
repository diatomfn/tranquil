#include "array.h"

#include "CHEL/common.h"

namespace JS {
    Array::Array(JsValueRef value) {
        this->value = value;
        this->GetArrayBindings();
    }

    JsValueRef Array::Push(JsValueRef value) {
        JsValueRef arguments[] = {this->value, value};

        JsValueRef result;

        if (JsCallFunction(this->push, arguments, 2, &result) != JsNoError)
            throw FatalRuntimeException();

        return this->length;
    }
        
    // JsValueRef Native::JSONParse(JsValueRef string) {
    //     JsValueRef arguments[] = {JS::Common::GetUndefined(), string};

    //     JsValueRef result;

    //     if (JsCallFunction(Native::jsonParse, arguments, 2, &result) != JsNoError)
    //         throw FatalRuntimeException();

    //     return result;
    // }

    void Array::GetArrayBindings() {
        JS::Object obj(this->value);

        this->length = obj.GetProperty("length");
        this->push = obj.GetProperty("push");
    }
}