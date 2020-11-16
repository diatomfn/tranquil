#pragma once

#include "pch.h"

#include "FatalRuntimeException.h"

namespace JS::Number {
    JsValueRef ToJS(int value);
    int FromJS(JsValueRef& value);
    JsValueRef ValueToNumber(JsValueRef& value);
}