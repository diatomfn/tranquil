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
    /**
     * @brief Get the type of a JS value as a string
     * 
     * @param type the javascript value
     * 
     * @return the type of the value as a string
     */
    const char* GetTypeString(JsValueType type);
    /**
     * @brief Assert function for checking function arguments
     *
     * @param value the function argument
     * @param type the expected value type
     * @param exception should assertion fail throw a JavaScript exception
     *
     * @return false if assertion errored and true if succeeded
     */
    bool AssertArgument(JsValueRef value, JsValueType type, bool exception);
}