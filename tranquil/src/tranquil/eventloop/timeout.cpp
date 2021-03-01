#include "timeout.h"
#include "tranquil/exceptions.h"

namespace tranquil {
    Timeout::Timeout(JsValueRef func, JsValueRef repeat, JsValueRef destroyed) : _onTimeout(func), _repeat(repeat), _destroyed(destroyed) {
        this->object = tranquil::Value::Object();

        object.SetProperty("_onTimeout", func);
        object.SetProperty("_repeat", repeat);
        object.SetProperty("_destroyed", destroyed);
    }

    Timeout::Timeout(JsValueRef object) {
        this->object = tranquil::Value(object);

        this->_onTimeout = this->object.GetProperty("_onTimeout");
        this->_repeat = this->object.GetProperty("_repeat");
        this->_destroyed = this->object.GetProperty("_destroyed");
    }

    void Timeout::Destroy() const {
        tranquil::Value obj(this->object);
        obj.SetProperty("_destroyed", tranquil::Value(true));
    }

    bool Timeout::Destroyed() const {
        tranquil::Value obj(this->object);
        return obj.GetProperty("_destroyed").AsBool();
    }

    void Timeout::SetRepeat(int repeat) const {
        tranquil::Value obj(this->object);
        obj.SetProperty("_repeat", tranquil::Value((double)repeat));
    }
}