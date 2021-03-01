#pragma once

#include "pch.h"

#include "context.h"

namespace tranquil {
    class Native {
    public:
        static void Init(Context& context);

        // Native function bindings
        static JsValueRef JSONStringify(JsValueRef object);
        static JsValueRef JSONParse(JsValueRef object);
        static JsValueRef ObjectAssign(JsValueRef destination, JsValueRef from);
        static JsValueRef ObjectValues(JsValueRef object);
    private:
        static JsValueRef GetFromJS(const std::string& identifier, Context&);

        // Store native function reference copies
        static thread_local JsValueRef jsonStringify;
        static thread_local JsValueRef jsonParse;
        static thread_local JsValueRef objectAssign;
        static thread_local JsValueRef objectValues;
    };
}
