#pragma once

#include "pch.h"
#include "value.h"
#include "FatalRuntimeException.h"

namespace JS {
    using Function = std::function<JsValueRef(bool, JS::Value, std::vector<JS::Value>, void*)>;

    /**
     * @brief FunctionData will be passed to FunctionWrapperInvoker as callbackstate.
     * Contains the function to call and its state
     */
    struct FunctionData {
        void*    functionState;
        Function function;
    };

    JsValueRef FunctionWrapperInvoker(JsValueRef call, bool isConstructCall, JsValueRef *args, unsigned short argumentCount, void* callbackState) {
        auto* data = static_cast<FunctionData*>(callbackState);

        JS::Value context = args[0];

        std::vector<Value> arglist;
        for (size_t i = 1; i < argumentCount - 1; i++)
            arglist.push_back(args[i]);

        return data->function(isConstructCall, context, arglist, data->functionState);
    }

    JS::Value CreateFunction(Function function, void* callbackState) {
        FunctionData functionData = {
            .functionState = callbackState,
            .function = function
        };

        JsValueRef functionRef;
        if (JsCreateFunction(FunctionWrapperInvoker, &functionData, &functionRef) != JsNoError)
            throw FatalRuntimeException();

        return JS::Value(functionRef);
    }
}