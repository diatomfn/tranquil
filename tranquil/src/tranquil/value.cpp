#include "value.h"

namespace tranquil {
    #pragma region Constructors

    Value::Value() {
        if (JsGetUndefinedValue(&this->value) != JsNoError)
            throw ValueException();
    }

    Value::Value(JsValueRef value) {
        this->value = value;
    }

    Value::Value(const std::string& value) {
        if (JsCreateString(value.c_str(), value.length(), &this->value) != JsNoError)
            throw ValueException();
    }

    Value::Value(double value) {
        if (JsDoubleToNumber(value, &this->value) != JsNoError)
            throw ValueException();
    }

    Value::Value(JsNativeFunction function, void* callbackState) {
        JsValueRef functionRef;
        if (JsCreateFunction(function, callbackState, &functionRef) != JsNoError)
            throw ValueException();

        this->value = Value(functionRef);
    }
    
    Value::Value(bool value) {
        if (JsBoolToBoolean(value, &this->value) != JsNoError)
            throw ValueException();
    }

    Value Value::Object() {
        JsValueRef value;
        if (JsCreateObject(&value))
            throw ValueException();
        return Value(value);
    }

    Value Value::Array() {
        JsValueRef value;
        if (JsCreateArray(0, &value) != JsNoError)
            throw ValueException();
        return Value(value);
        // Need to setup push/pop/length for array
    }

    Value Value::Promise() {
        Value promise = Value(JS_INVALID_REFERENCE);
        if (JsCreatePromise(&promise.value, &promise.promiseResolveFunc, &promise.promiseRejectFunc) != JsNoError)
            throw ValueException();
        return promise;
    }

    Value Value::GetGlobalObject() {
        JsValueRef global;
        if (JsGetGlobalObject(&global) != JsNoError)
            throw ValueException();
        return Value(global); 
    }

    #pragma endregion Constructors

    #pragma region Promise

    JsPromiseState Value::PromiseState() {
        JsPromiseState state;
        // This will throw if the value is not a promise
        if (JsGetPromiseState(this->value, &state))
            throw InvalidTypeException();
        return state;
    }

    void Value::PromiseResolve(Value value) {
        if (!this->IsPromise())
            throw InvalidTypeException();
        
        if (!this->promiseResolveFunc)
            throw PromiseNotOriginException();

        JsValueRef arguments[] = {this->value, value};

        JsValueRef result;

        if (JsCallFunction(this->promiseResolveFunc, arguments, 2, &result) != JsNoError)
            throw ValueException();
    }

    void Value::PromiseReject(Value value) {
        if (!this->IsPromise())
            throw InvalidTypeException();

        if (!this->promiseRejectFunc)
            throw PromiseNotOriginException();

        JsValueRef arguments[] = {this->value, value};

        JsValueRef result;

        if (JsCallFunction(this->promiseRejectFunc, arguments, 2, &result) != JsNoError)
            throw ValueException();
    }

    Value Value::PromiseResult() {
        JsValueRef result;
        if (JsGetPromiseResult(value, &result) != JsNoError)
            throw ValueException();
        return Value(result);
    }

    #pragma endregion Promise

    JsValueType Value::GetType() {
        JsValueType type;
        if (JsGetValueType(value, &type) != JsNoError)
            throw ValueException();
        return type;
    }

    JsValueType Value::GetType(JsValueRef value) {
        JsValueType type;
        if (JsGetValueType(value, &type) != JsNoError)
            throw ValueException();
        return type;
    }

    Value Value::Invoke(std::vector<JsValueRef>& arguments) {
        JsValueRef result;
        if (JsCallFunction(this->value, &arguments[0], arguments.size(), &result))
            throw ValueException();
        return result;
    }

    Value Value::Invoke(std::vector<tranquil::Value>& arguments) {
        // Convert JS::Value vec to JsValueRef
        // Happens implicitly anyway in JsValueRef vec unless actually JsValueRef
        std::vector<JsValueRef> jsRefVec;
        for (int i = 0; i < arguments.size(); i++) {
            jsRefVec.emplace_back((JsValueRef)arguments[i]);
        }

        JsValueRef result;
        if (JsCallFunction(this->value, &jsRefVec[0], arguments.size(), &result))
            throw ValueException();
        return result;
    }

    Value::operator JsValueRef () const {
        return this->value;
    }

    #pragma region Property

    Value Value::GetIndex(int index) {
        JsValueRef result;
        if (JsGetIndexedProperty(this->value, Value((double)index), &result) != JsNoError)
            throw ValueException();
        return result;
    }

    void Value::SetIndex(int index, JsValueRef value) {
        if (JsSetIndexedProperty(this->value, Value((double)index), value) != JsNoError)
            throw ValueException();
    }

    Value Value::GetProperty(JsValueRef key) {
        JsValueRef result;
        if (JsObjectGetProperty(this->value, key, &result) != JsNoError)
            throw ValueException();
        return result;
    }

    Value Value::GetProperty(const std::string& key) {
        return GetProperty(Value(key.c_str()));
    }
    
    void Value::SetProperty(JsValueRef key, JsValueRef value) {
        if (JsObjectSetProperty(this->value, key, value, false) != JsNoError)
            throw ValueException();
    }

    void Value::SetProperty(const std::string& key, JsValueRef value) {
        SetProperty(Value(key), value);
    }

    void Value::ArrayPush(JsValueRef value) {
        SetIndex(ArrayLength(), value);
    }

    void Value::ArrayPop() {
        int length = GetProperty("length").AsDouble();
        if (length == 0) return;
        SetProperty("length", Value((double)length - 1));
    }

    int Value::ArrayLength() {
        return GetProperty("length").AsDouble();
    }

    #pragma endregion Property

    #pragma region Getters

    std::string Value::AsString(JsValueRef value) {
        Value jsString = ToString(value);
 
        char* str = nullptr;
        size_t length;

        // get the length of the value
        if (JsCopyString(jsString, nullptr, 0, &length) != JsNoError)
            throw ValueException();

        str = (char*) malloc(length + 1);

        // get the value as a string
        if (JsCopyString(jsString, str, length+1, nullptr) != JsNoError) {
            free(str);
            throw ValueException();
        }

        str[length] = 0;

        std::string rVal(str);

        free(str);

        return rVal;
    }

    bool Value::AsBool() {
        bool temp;
        if (JsBooleanToBool(this->value, &temp) != JsNoError)
            throw ValueException();
        return temp;
    }

    double Value::AsDouble() {
        double temp;
        if (JsNumberToDouble(this->value, &temp) != JsNoError)
            throw ValueException();
        return temp;
    }

    std::string Value::AsString() {
        return AsString(this->value);
    }

    #pragma endregion Getters

    #pragma region Conversion

    Value Value::ToString() {
        return Value(ToString(this->value));
    }

    Value Value::ToNumber() {
        return Value(ToNumber(this->value));
    }

    Value Value::ToBoolean() {
        return Value(ToBoolean(this->value));
    }

    Value Value::ToArray() {
        return Value(ToArray(this->value));
    }

    Value Value::ToObject() {
        return Value(ToObject(this->value));
    }

    JsValueRef Value::ToString(JsValueRef value) {
        JsValueRef temp;
        if (JsConvertValueToString(value, &temp))
            throw ValueException();
        return temp;
    }

    JsValueRef Value::ToNumber(JsValueRef value) {
        JsValueRef temp;
        if (JsConvertValueToNumber(value, &temp))
            throw ValueException();
        return temp;
    }

    JsValueRef Value::ToBoolean(JsValueRef value) {
        JsValueRef temp;
        if (JsConvertValueToBoolean(value, &temp))
            throw ValueException();
        return temp;
    }

    JsValueRef Value::ToObject(JsValueRef value) {
        JsValueRef temp;
        if (JsConvertValueToObject(value, &temp))
            throw ValueException();
        return temp;
    }

    JsValueRef Value::ToArray(JsValueRef value) {
        return Native::ObjectValues(value);
    }

    #pragma endregion Conversion

    #pragma region Comparison

    bool Value::IsNumber() {
        return GetType() == JsNumber;
    }

    bool Value::IsString() {
        return GetType() == JsString;
    }

    bool Value::IsObject() {
        return GetType() == JsObject;
    }

    bool Value::IsArray() {
        return GetType() == JsArray;
    }

    bool Value::IsBoolean() {
        return GetType() == JsBoolean;
    }

    bool Value::IsFunction() {
        return GetType() == JsFunction;
    }

    bool Value::IsPromise() {
        try {
            this->PromiseState();
        } catch (...) {
            return false;
        }
        return true;
    }

    #pragma endregion Comparison
}
