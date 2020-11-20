#include "timeout.h"

namespace JS {
    Timeout::Timeout(JsValueRef func, JsValueRef repeat, JsValueRef destroyed) : _onTimeout(func), _repeat(repeat), _destroyed(destroyed) {
        this->object = JS::Object();

        object.SetProperty("_onTimeout", func);
        object.SetProperty("_repeat", repeat);
        object.SetProperty("_destroyed", destroyed);
    }

    Timeout::Timeout(JsValueRef object) {
        this->object = JS::Object(object);

        this->_onTimeout = this->object.GetProperty("_onTimeout");
        this->_repeat = this->object.GetProperty("_repeat");
        this->_destroyed = this->object.GetProperty("_destroyed");

    }

    void Timeout::Destroy() const {
        JS::Object obj(this->object);
        obj.SetProperty("_destroyed", JS::Boolean(true));
    }

    bool Timeout::Destroyed() const {
        JS::Object obj(this->object);
        return JS::Boolean(obj.GetProperty("_destroyed"));
    }

    void Timeout::SetRepeat(int repeat) const {
        object.SetProperty("_repeat", JS::Number(repeat));
    }
}