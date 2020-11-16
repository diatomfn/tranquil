#pragma once

#include "pch.h"

#include "FatalRuntimeException.h"

namespace JS::String {
    JsValueRef ToJS(const char* str);
    std::string FromJS(JsValueRef& value);
    JsValueRef ValueToStringJS(JsValueRef& value);
}