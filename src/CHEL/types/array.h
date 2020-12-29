#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

#include "value.h"
#include "object.h"
#include "number.h"

namespace JS {
    class Array : public Value {
    public:
        using Value::Value;
        using Value::operator JsValueRef;
        /**
         * @brief Construct an Array object wrapper from a Javascript value
         * 
         * @param value Javascript value
         */
        explicit Array(JsValueRef value);

        /**
         * @brief Construct an empty Javascript Array object
         */
        Array();

        /**
         * @brief Setup the method references for an array object
         * Only required if array is constructed with an existing Js value
         */
        void SetupBindings();

        /**
         * @brief Push a value to the Javascript array
         * 
         * @param value the value to push
         * @return the new length of the array
         */
        JS::Value Push(JsValueRef value);

        /**
         * @brief Pop off the last element of the array
         * 
         * @return the element that was removed from the array 
         */
        JS::Value Pop();

        /**
         * @brief Get the length of the array
         * 
         * @return length
         */
        JS::Number Length();

        JS::Value operator[] (int index);
        JS::Value operator[] (JsValueRef index);
    private:
        JsValueRef length;
        JsValueRef push;
        JsValueRef pop;
    };
}