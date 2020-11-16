#pragma once

#include "pch.h"

#include "FatalRuntimeException.h"

namespace JS::Boolean {
    bool FromJS(JsValueRef value);
    JsValueRef ToJS(bool value);
}