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
         * @brief Construct a new empty JavaScript object
         */
        explicit Object();
        /**
         * @brief Retrieve global object from current runtime thread
         * Must have a current runtime active
         * 
         * @return JS::Object global object 
         */
        static JS::Object GetGlobalObject();
        /**
         * @brief Get a property of a JavaScript object
         *
         * @param key the key of the property
         *
         * @return the Javascript property value
         */
        JS::Value GetProperty(JS::String key);
        JS::Value GetProperty(const char* key);
        /**
         * @brief Set a property of a JavaScript object
         *
         * @param key the key of the property
         * @param value the value of the property
         */
        void SetProperty(JS::String key, JsValueRef value) const;
        void SetProperty(const char* key, JsValueRef value) const;
        /**
         * @brief Set a function property of a JavaScript object
         *
         * @param key the key of the property
         * @param callback the function to call
         * @param moduleInstance the passed callback state
         */
        void SetProperty(JS::String key, JsNativeFunction callback, void* moduleInstance = nullptr) const;
        void SetProperty(const char* key, JsNativeFunction callback, void* moduleInstance = nullptr) const;
    };
}