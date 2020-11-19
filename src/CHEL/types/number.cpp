#include "number.h"

namespace JS {
    Number::Number(double number) {
        if (JsDoubleToNumber(number, &this->value) != JsNoError)
            throw FatalRuntimeException();
    }

    double Number::FromJS() {
        double num = 0;

        if (JsNumberToDouble(this->value, &num) != JsNoError)
            throw FatalRuntimeException();

        return num;
    }
}