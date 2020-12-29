#include "object.h"

namespace JS {
    Object::Object() {
        if (JsCreateObject(&this->value))
            throw FatalRuntimeException();
    }

    JS::Value Object::GetProperty(JS::String key) {
        JsValueRef propertyValue;

        if (JsObjectGetProperty(this->value, key, &propertyValue) != JsNoError)
            throw FatalRuntimeException();

        return JS::Value(propertyValue);
    }

    JS::Value Object::GetProperty(const char *key) {
        JsValueRef propertyValue;
        JS::String jKey(key);

        if (JsObjectGetProperty(this->value, jKey, &propertyValue) != JsNoError)
            throw FatalRuntimeException();

        return JS::Value(propertyValue);
    }

    void Object::SetProperty(JS::String key, JsValueRef value) const {
        if (JsObjectSetProperty(this->value, key, value, false) != JsNoError)
            throw FatalRuntimeException();
    }

    void Object::SetProperty(const char *key, JsValueRef value) const {
        if (JsObjectSetProperty(this->value, JS::String(key), value, false) != JsNoError)
            throw FatalRuntimeException();
    }

    void Object::SetProperty(JS::String key, JsNativeFunction callback, void *moduleInstance) const {
        JsValueRef function;

        if (JsCreateFunction(callback, moduleInstance, &function))
            throw FatalRuntimeException();

        JsObjectSetProperty(this->value, key, function, true);
    }

    void Object::SetProperty(const char *key, JsNativeFunction callback, void *moduleInstance) const {
        JsValueRef function;
        JS::String jKey(key);

        if (JsCreateFunction(callback, moduleInstance, &function))
            throw FatalRuntimeException();

        JsObjectSetProperty(this->value, jKey, function, true);
    }

    JS::Object JS::Object::GetGlobalObject() {
       JsValueRef global;
        if (JsGetGlobalObject(&global) != JsNoError)
            throw FatalRuntimeException();
        return JS::Object(global); 
    }
}