#pragma once

#include "pch.h"

#include "runtime.h"
#include "native.h"

namespace JS {
    class Object {
    public:
        explicit Object(JsValueRef ref) : jsObject(ref) {};

        JsValueRef GetProperty(const std::string& key) const;
        void SetProperty(const std::string& key, JsValueRef value) const;

        void SetFunctionProperty(const char* key, JsNativeFunction callback, void* moduleInstance = nullptr) const;

        JsValueRef jsObject;
    };
}