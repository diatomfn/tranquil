#pragma once

#include "pch.h"
#include "FatalRuntimeException.h"

namespace JS::Common {
    JsValueRef GetUndefined();
    JsValueRef GetGlobalObject();
    JsValueType GetValueType(JsValueRef value);
    bool IsPromise(JsValueRef value);
    JsValueRef GetNewObject();
    JsValueRef CreateFunction(JsNativeFunction function, void* callbackState);
}