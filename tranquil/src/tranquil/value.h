#pragma once

#include "native.h"

/**
 * @brief Adapter for tranquil method to be accepted as a CC function pointer
 */
#define ADAPT_METHOD(T, F) [](JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) { \
    std::vector<tranquil::Value> arglist; \
    for (size_t i = 1; i < argumentCount; i++) \
        arglist.push_back(arguments[i]); \
    return (JsValueRef)std::invoke((F), static_cast<std::add_pointer_t<T>>(callbackState), isConstructCall, arguments[0], std::move(arglist)); }

/**
 * @brief Adapter for tranquil function to be accepted as a CC function pointer
 */
#define ADAPT_FN(F) [](JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState) { \
    std::vector<tranquil::Value> arglist; \
    for (size_t i = 1; i < argumentCount; i++) \
        arglist.push_back(arguments[i]); \
    return (JsValueRef)std::invoke((F), isConstructCall, arguments[0], std::move(arglist), callbackState); }

namespace tranquil {
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
            return "This promise was created through a reference, PromiseResolve and PromiseReject methods are unavailable for this value";
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
         * @brief Create a new JavaScript value from a string
         * 
         * @param value string
         */
        Value(const std::string& value);

        /**
         * @brief Create a new JavaScript value from a number
         * 
         * @param value number value
         */
        Value(double value);

        /**
         * @brief Create a new function value from a C function
         * 
         * @param function
         * @param callbackState value to pass to function
         */
        Value(JsNativeFunction function, void* callbackState);

        /**
         * @brief Create a new JavaScript boolean.
         * Must be explicit because of constructor issues
         * 
         * @param value boolean value
         */
        explicit Value(bool value);

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
         * @brief Create a reference to the current context's global object
         * 
         * @return Value reference to global object
         */
        static Value GetGlobalObject();

        /**
         * @return value as string value
         */
        Value ToString();

        /**
         * @return value as number value
         */
        Value ToNumber();

        /**
         * @return value as boolean value
         */
        Value ToBoolean();

        /**
         * @return value as array value
         */
        Value ToArray();

        /**
         * @return value as object value
         */
        Value ToObject();

        // Promise stuff
        void PromiseResolve(Value value);
        void PromiseReject(Value value);

        /**
         * @brief Get the result of a promise.
         * This will work if the promise is created through reference made in JS.
         * 
         * @return The result of the promise
         */
        Value PromiseResult();

        JsPromiseState PromiseState();

        // Array methods
        void ArrayPush(JsValueRef value);
        void ArrayPop();
        int ArrayLength();

        /**
         * @return The value as a bool
         */
        bool AsBool();

        /**
         * @return The value as a double
         */
        double AsDouble();

        /**
         * @return The value as a string
         */
        std::string AsString();

        bool IsNumber();
        bool IsString();
        bool IsObject();
        bool IsArray();
        bool IsBoolean();
        bool IsPromise();
        bool IsFunction();

        /**
         * @brief Get the value at a numbered index of an object
         * 
         * @param index to get
         * @return Value at the index
         */
        Value GetIndex(int index);

        /**
         * @brief Set the value at a numbered index of an object
         * 
         * @param index to set
         * @param value Value to set the index to
         */
        void SetIndex(int index, JsValueRef value);

        /**
         * @brief Get the property of an object
         * 
         * @param key to get value for
         */
        Value GetProperty(const std::string& key);

        /**
         * @brief Get the property of an object
         * 
         * @param key to get value for
         */
        Value GetProperty(JsValueRef key);

        /**
         * @brief Set the property of an object
         * 
         * @param key to get value for
         * @param value to set
         */
        void SetProperty(const std::string& key, JsValueRef value);
        
        /**
         * @brief Set the property of an object
         * 
         * @param key to get value for
         * @param value to set
         */
        void SetProperty(JsValueRef key, JsValueRef value);

        /**
         * @brief Get the type of a value
         * 
         * @return JsValueType 
         */
        JsValueType GetType();

        /**
         * @brief Invoke function and get result.
         * Value must be a function.
         * 
         * @param arguments to pass to function
         */
        Value Invoke(std::vector<JsValueRef>& arguments);

        /**
         * @brief Invoke function and get result.
         * Value must be a function.
         * 
         * @param arguments to pass to function
         */
        Value Invoke(std::vector<tranquil::Value>& arguments);

        operator JsValueRef () const;
    private:
        static JsValueRef ToString(JsValueRef value);
        static JsValueRef ToNumber(JsValueRef value);
        static JsValueRef ToBoolean(JsValueRef value);
        static JsValueRef ToArray(JsValueRef value);
        static JsValueRef ToObject(JsValueRef value);

        static std::string AsString(JsValueRef value);

        static JsValueType GetType(JsValueRef value);

        // Promise return functions
        JsValueRef promiseResolveFunc;
        JsValueRef promiseRejectFunc;

        JsValueRef value;
    };
}
