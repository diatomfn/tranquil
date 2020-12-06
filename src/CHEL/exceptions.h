#pragma once

#include "runtime.h"
#include "pch.h"

namespace JS::Exceptions {
    void InvalidArgument(JsValueType type);
    void ClassNewKeyword(const char* className);
}