#include "timeout.h"

namespace JS {
    Timeout::Timeout(JsValueRef func, JsValueRef repeat, JsValueRef destroyed) : _onTimeout(func), _repeat(repeat), _destroyed(destroyed) {
        this->object = JS::Value::Object();

        object.SetProperty("_onTimeout", func);
        object.SetProperty("_repeat", repeat);
        object.SetProperty("_destroyed", destroyed);
    }

    Timeout::Timeout(JsValueRef object) {
        this->object = JS::Value(object);

        this->_onTimeout = this->object.GetProperty("_onTimeout");
        this->_repeat = this->object.GetProperty("_repeat");
        this->_destroyed = this->object.GetProperty("_destroyed");
    }

    void Timeout::Destroy() const {
        JS::Value obj(this->object);
        obj.SetProperty("_destroyed", JS::Value(true));
    }

    bool Timeout::Destroyed() const {
        JS::Value obj(this->object);
        return obj.GetProperty("_destroyed").AsBool();
    }

    void Timeout::SetRepeat(int repeat) const {
        object.SetProperty("_repeat", JS::Value(repeat));
    }
}