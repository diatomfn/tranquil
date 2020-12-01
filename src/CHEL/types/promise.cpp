#include "promise.h"

#include "CHEL/common.h"

namespace JS {
    Promise::Promise() {
        if (JsCreatePromise(&this->value, &this->resolveFunc, &this->rejectFunc) != JsNoError)
            throw FatalRuntimeException();
    }

    void Promise::Resolve(JsValueRef value) {
        JsValueRef arguments[] = {this->value, value};

        JsValueRef result;

        if (JsCallFunction(this->resolveFunc, arguments, 2, &result) != JsNoError)
            throw FatalRuntimeException();
    }

    void Promise::Reject(JsValueRef value) {
        JsValueRef arguments[] = {this->value, value};

        JsValueRef result;

        if (JsCallFunction(this->rejectFunc, arguments, 2, &result) != JsNoError)
            throw FatalRuntimeException();
    }
}