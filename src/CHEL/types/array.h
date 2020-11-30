#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

#include "value.h"
#include "object.h"

namespace JS {
    class Array : public Value {
    public:
        using Value::operator JsValueRef;

        explicit Array(JsValueRef value);

        JsValueRef Push(JsValueRef value);
        void Pop();
    private:
        void GetArrayBindings();

        JsValueRef length;
        JsValueRef push;
    };
}