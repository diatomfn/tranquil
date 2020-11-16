#include "timeout.h"

#include "CHEL/object.h"
#include "CHEL/boolean.h"
#include "CHEL/common.h"
#include "CHEL/number.h"

namespace JS {
    Timeout::Timeout(JsValueRef func, JsValueRef repeat, JsValueRef destroyed) : _onTimeout(func), _repeat(repeat), _destroyed(destroyed) {
        this->object = JS::Common::GetNewObject();

        JS::Object obj(object);
        obj.SetProperty("_onTimeout", func);
        obj.SetProperty("_repeat", repeat);
        obj.SetProperty("_destroyed", destroyed);
    }

    Timeout::Timeout(JsValueRef object) {
        this->object = object;

        JS::Object obj(object);
        this->_onTimeout = obj.GetProperty("_onTimeout");
        this->_repeat = obj.GetProperty("_repeat");
        this->_destroyed = obj.GetProperty("_destroyed");
    }

    void Timeout::Destroy() const {
        JS::Object obj(this->object);
        obj.SetProperty("_destroyed", JS::Boolean::ToJS(true));
    }

    bool Timeout::Destroyed() const {
        JS::Object obj(this->object);
        return JS::Boolean::FromJS(obj.GetProperty("_destroyed"));
    }

    void Timeout::SetRepeat(int repeat) const {
        JS::Object obj(this->object);
        obj.SetProperty("_repeat", JS::Number::ToJS(repeat));
    }
}