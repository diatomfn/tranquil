#include "common.h"

namespace JS::Common {
    JsValueRef GetUndefined() {
        JsValueRef undefined;
        if (JsGetUndefinedValue(&undefined) != JsNoError)
            throw FatalRuntimeException();
        return undefined;
    }
    JsValueRef GetGlobalObject() {
        JsValueRef global;
        if (JsGetGlobalObject(&global) != JsNoError)
            throw FatalRuntimeException();
        return global;
    }
    JsValueType GetType(JsValueRef value) {
        JsValueType type;
        if (JsGetValueType(value, &type) != JsNoError)
            throw FatalRuntimeException();
        return type;
    }
    // If JsGetPromiseState does not error then it is a promise
    bool IsPromise(JsValueRef value) {
        JsPromiseState state;
        if (JsGetPromiseState(value, &state) != JsNoError)
            return false;
        return true;
    }
    JsValueRef CreateFunction(JsNativeFunction function, void* callbackState) {
        JsValueRef ret;
        if (JsCreateFunction(function, callbackState, &ret) != JsNoError)
            throw FatalRuntimeException();
        return ret;
    }
    const char* GetTypeString(JsValueType type) {
        switch (type) {
            case JsString:
                return "string";
            case JsNumber:
                return "number";
            case JsFunction:
                return "function";
            case JsNull:
                return "null";
            case JsUndefined:
                return "undefined";
            case JsError:
                return "error";
            case JsObject:
                return "object";
            case JsArray:
                return "array";
            case JsArrayBuffer:
                return "arraybuffer";
            case JsTypedArray:
                return "typedarray";
            case JsBoolean:
                return "boolean";
            case JsDataView:
                return "dataview";
            case JsSymbol:
                return "symbol";
            default:
                return "value";
        }
    }
    bool AssertArgument(JsValueRef value, JsValueType type, bool exception) {
        if (JS::Common::GetType(value) != type) {
            if (exception)
                JS::Runtime::ThrowException(std::string("Argument type is invalid, expected ").append(JS::Common::GetTypeString(type)).c_str());
            return false;
        }
        return true;
    }
    bool AssertConstructor(bool constructor, bool wanted, bool exception) {
        if (constructor != wanted) {
            JS::Runtime::ThrowException("Class constructor cannot be called without the new keyword");
            return false;
        }
        return true;
    }
}