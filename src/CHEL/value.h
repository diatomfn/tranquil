#pragma once

#include "pch.h"
#include "native.h"

namespace JS {
    class ValueException : public std::exception {
        const char* what() const throw() {
            return "JavaScript value could not be modified or instantiated";
        }
    };

    class InvalidTypeException : public ValueException {
        const char* what() const throw() {
            return "This operation cannot be performed on a value of this type";
        }
    };

    class PromiseNotOriginException : public ValueException {
        const char* what() const throw() {
            return "This promise was created through a reference, you may only use PromiseResolve and PromiseReject methods are unavailable for this value";
        }
    };

    class Value {
    public:
        /**
         * @brief Create an undefined value
         */
        Value();

        /**
         * @brief Copy a JS value into this value.
         * If the value is an immutable primative then it will be copied.
         * If it is an object it will reference as long as it is not converted.
         * 
         * @param value JavaScript value
         */
        Value(JsValueRef value);

        /**
         * @brief Create a new JavaScript value from a CString
         * 
         * @param value CString
         */
        Value(const std::string& value);

        /**
         * @brief Create a new JavaScript value from a number
         * 
         * @param value number value
         */
        Value(double value);

        /**
         * @brief Create a new empty JavaScript object
         */
        static Value Object();

        /**
         * @brief Create a new empty JavaScript array
         */
        static Value Array();

        /**
         * @brief Create a promise.
         * Promises can only be constructed here, no value can be converted to a promise unless provided as reference
         */
        static Value Promise();

        /**
         * @brief Create a reference to the global JS context object
         * 
         * @return Value reference to global context
         */
        static Value GetGlobalContext();

        /**
         * @brief Value conversions
         * 
         * @return true if conversion suceeded
         * @return false if conversion failed
         */
        void ToString();
        void ToNumber();
        void ToBoolean();
        void ToArray();
        void ToObject();

        // Promise stuff
        void PromiseResolve(Value value);
        void PromiseReject(Value value);
        Value PromiseResult();
        JsPromiseState PromiseState();

        // Array methods
        void ArrayPush(JsValueRef value);
        void ArrayPop();
        int ArrayLength();

        /**
         * @brief Value conversions to C types
         * 
         * @return The value as a C++ type 
         */
        bool AsBool();
        double AsDouble();
        std::string AsString();

        bool IsNumber();
        bool IsString();
        bool IsObject();
        bool IsArray();
        bool IsBoolean();
        bool IsPromise();

        // Must be called on object or array
        Value GetIndex(int index);
        void SetIndex(int index, JsValueRef value);

        Value GetProperty(const std::string& key);
        Value GetProperty(JsValueRef key);
        void SetProperty(const std::string& key, JsValueRef value);
        void SetProperty(JsValueRef key, JsValueRef value);

        JsValueType GetType();
        static JsValueType GetType(JsValueRef value);

        operator JsValueRef () const;
    private:
        static JsValueRef ToString(JsValueRef value);
        static JsValueRef ToNumber(JsValueRef value);
        static JsValueRef ToBoolean(JsValueRef value);
        static JsValueRef ToArray(JsValueRef value);
        static JsValueRef ToObject(JsValueRef value);

        static std::string AsString(JsValueRef value);

        // Promise return functions
        JsValueRef promiseResolveFunc;
        JsValueRef promiseRejectFunc;

        JsValueRef value;
    };
}
