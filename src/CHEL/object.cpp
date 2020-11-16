#include "object.h"

#include "string.h"

#include "module.h"
#include "native.h"

namespace JS {
    JsValueRef Object::GetProperty(const std::string& key) const {
        JsPropertyIdRef keyID;
        if (JsCreatePropertyId(key.c_str(), key.length() - 1, &keyID) != JsNoError)
            throw FatalRuntimeException();

        JsValueRef propertyValue;
        if (JsObjectGetProperty(this->jsObject, JS::String::ToJS(key.c_str()), &propertyValue) != JsNoError)
            throw FatalRuntimeException();

        return propertyValue;
    }

    void Object::SetProperty(const std::string &key, JsValueRef value) const {
        if (JsObjectSetProperty(this->jsObject, JS::String::ToJS(key.c_str()), value, false) != JsNoError)
            throw FatalRuntimeException();
    }

    void Object::SetFunctionProperty(const char* key, JsNativeFunction callback, void* moduleInstance) const {
        JsValueRef function;
        JsCreateFunction(callback, moduleInstance, &function);

        JsObjectSetProperty(this->jsObject, JS::String::ToJS(key), function, true);
    }
}