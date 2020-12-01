#include "promise.h"

#include "CHEL/common.h"

namespace JS {
    Promise::Promise() {
        if (JsCreatePromise(&this->value, &this->resolveFunc, &this->rejectFunc) != JsNoError)
            throw FatalRuntimeException();
    }

    JS::Value Promise::GetPromiseResult(JsValueRef value) {
        JsValueRef result;
        if (JsGetPromiseResult(value, &result) != JsNoError)
            throw FatalRuntimeException();
        return JS::Value(result);
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

    JsPromiseState Promise::GetState() {
        JsPromiseState state;
        // This will throw if the value is not a promise
        if (JsGetPromiseState(this->value, &state))
            throw FatalRuntimeException();
        return state;
    }

    JsPromiseState Promise::GetState(JsValueRef value) {
        JsPromiseState state;
        // This will throw if the value is not a promise
        if (JsGetPromiseState(value, &state))
            throw FatalRuntimeException();
        return state;
    }
}