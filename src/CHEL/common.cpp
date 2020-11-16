#include "common.h"

namespace JS::Common {
    JsValueRef GetUndefined() {
        JsValueRef undefined;
        if (JsGetUndefinedValue(&undefined) != JsNoError)
            throw FatalRuntimeException();
        return undefined;
    }
    JsValueRef GetGlobalObject() {
        JsValueRef global;
        if (JsGetGlobalObject(&global) != JsNoError)
            throw FatalRuntimeException();
        return global;
    }
    JsValueType GetValueType(JsValueRef value) {
        JsValueType type;
        if (JsGetValueType(value, &type) != JsNoError)
            throw FatalRuntimeException();
        return type;
    }
    JsValueRef GetNewObject() {
        JsValueRef object;
        if (JsCreateObject(&object) != JsNoError)
            throw FatalRuntimeException();
        return object;
    }
    // If JsGetPromiseState does not error then it is a promise
    bool IsPromise(JsValueRef value) {
        JsPromiseState state;
        if (JsGetPromiseState(value, &state) != JsNoError)
            return false;
        return true;
    }
    JsValueRef CreateFunction(JsNativeFunction function, void* callbackState) {
        JsValueRef ret;
        if (JsCreateFunction(function, callbackState, &ret) != JsNoError)
            throw FatalRuntimeException();
        return ret;
    }
}