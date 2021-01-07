#include "common.h"

#include "types/value.h"
#include "exceptions.h"

namespace JS::Common {
    // If JsGetPromiseState does not error then it is a promise
    bool IsPromise(JsValueRef value) {
        JsPromiseState state;
        if (JsGetPromiseState(value, &state) != JsNoError)
            return false;
        return true;
    }
    JS::Value CreateFunction(JsNativeFunction function, void* callbackState) {
        JsValueRef ret;
        if (JsCreateFunction(function, callbackState, &ret) != JsNoError)
            throw FatalRuntimeException();
        return JS::Value(ret);
    }
    bool AssertArgument(JsValueRef value, JsValueType type, bool exception) {
        if (JS::Value::GetType(value) != type) {
            if (exception) JS::Exceptions::InvalidArgument();
            return false;
        }
        return true;
    }
}