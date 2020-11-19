#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

#include "value.h"
#include "string.h"

namespace JS {
    class Object : public Value {
    public:
        using Value::Value;
        using Value::operator JsValueRef;
        /**
         * Create a new JavaScript object
         */
        explicit Object();
        /**
         * Get a property of a JavaScript object
         *
         * @param key the key of the property
         *
         * @return JavaScript property value
         */
        JS::Value GetProperty(JS::String key);
        JS::Value GetProperty(const char* key);
        /**
         * Set a property of a JavaScript object
         *
         * @param key the key of the property
         * @param value the value of the property
         */
        void SetProperty(JS::String key, JsValueRef value) const;
        void SetProperty(const char* key, JsValueRef value) const;
        /**
         * Set a function property of a JavaScript object
         *
         * @param key the key of the property
         * @param callback the function to call
         * @param moduleInstance the passed callback state
         */
        void SetProperty(JS::String key, JsNativeFunction callback, void* moduleInstance = nullptr) const;
        void SetProperty(const char* key, JsNativeFunction callback, void* moduleInstance = nullptr) const;
    };
}