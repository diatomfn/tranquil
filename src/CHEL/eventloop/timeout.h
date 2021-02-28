#pragma once

#include "CHEL/pch.h"
#include "CHEL/value.h"

namespace JS {
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

        JS::Object object;
    };
}