#pragma once

#include "tranquil/pch.h"
#include "tranquil/value.h"

namespace tranquil {
    class Timeout {
    public:
        // Create a new timeout
        Timeout(JsValueRef func, JsValueRef repeat, JsValueRef destroyed);
        // Create a timeout from an object
        explicit Timeout(JsValueRef object);

        void Destroy() const;
        bool Destroyed() const;

        void SetRepeat(int repeat) const;

        JsValueRef _onTimeout;
        JsValueRef _repeat;
        JsValueRef _destroyed;

        tranquil::Value object;
    };
}