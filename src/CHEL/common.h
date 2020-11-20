#pragma once

#include "pch.h"
#include "FatalRuntimeException.h"

#include "runtime.h"

namespace JS::Common {
    JsValueRef GetUndefined();
    JsValueRef GetGlobalObject();
    bool IsPromise(JsValueRef value);
    JsValueRef CreateFunction(JsNativeFunction function, void* callbackState);
    JsValueType GetType(JsValueRef value);
    const char* GetTypeString(JsValueType type);
    /*
     * Assert function for checking function arguments
     *
     * @param value the function argument
     * @param type the expected function type
     * @param exception should assertion fail throw a JavaScript exception
     *
     * @return returns assertion result
     */
    bool AssertArgument(JsValueRef value, JsValueType type, bool exception);
    /*
     * Assert function for checking if function is a constructor
     *
     * @param constructor the constructor bool from the function
     * @param wanted if it is expected to be a constructor
     * @param exception should assertion fail throw a JavaScript exception
     *
     * @return returns assertion result
     */
    bool AssertConstructor(bool constructor, bool wanted, bool exception);
}