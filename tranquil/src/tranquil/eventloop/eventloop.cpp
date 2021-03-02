#include "eventloop.h"

#include "tranquil/exceptions.h"

namespace tranquil {
    void EventLoop::InitPromiseCallback() {
        if (JsSetPromiseContinuationCallback(PromiseCallback, this) != JsNoError)
            throw FatalRuntimeException();
    }

    void EventLoop::PromiseCallback(JsValueRef task, void* callbackState) {
        tranquil::Value global = tranquil::Value::GetGlobalObject();
        auto* queue = static_cast<std::queue<Task*>*>(callbackState);

        tranquil::Timeout timeout(task, tranquil::Value(0.0), tranquil::Value(false));
        queue->push(new Task(timeout.object, global, JS_INVALID_REFERENCE, false));
    }

    void EventLoop::HandleException(JsValueRef except) {
        JsValueRef jsException;

        if (JsGetAndClearException(&jsException) != JsNoError)
            throw FatalRuntimeException();

        this->errorCallback(tranquil::Value(jsException));
    }

    void EventLoop::SetErrorCallback(std::function<void(tranquil::Value)> callback) {
        this->errorCallback = callback;
    }

    void EventLoop::AddTask(Task* task) {
        this->taskQueue.push(task);
    }

    void EventLoop::Loop() {
        while (!taskQueue.empty()) {
            Task* task = taskQueue.front();
            taskQueue.pop();
            auto timeout = tranquil::Timeout(task->timeout);

            int currentTime = (int)(clock() / (CLOCKS_PER_SEC / (double)1000));
            if (!tranquil::Value(timeout._destroyed).AsBool()) {
                if (currentTime-task->time > (int)(tranquil::Value(timeout._repeat).AsDouble())) {
                    JsValueRef taskResult = task->Invoke();
                    if (task->excepted) this->HandleException(taskResult);
                    if (task->repeat) {
                        task->time = currentTime;
                        taskQueue.push(task);
                    } else {
                        timeout.Destroy();
                        delete task;
                    }
                } else {
                    taskQueue.push(task);
                }
            } else {
                timeout.Destroy();
                delete task;
            }
        }
    }

    JsValueRef EventLoop::SetTimeout(bool isConstructor, tranquil::Value context, std::vector<tranquil::Value> args) {
        if (isConstructor || args.size() < 2)
            return JS_INVALID_REFERENCE;
        
        tranquil::Value func = args[0];
        tranquil::Value time = args[1];

        if (func.GetType() != JsFunction && time.GetType() != JsNumber) {
            tranquil::Exceptions::InvalidArgument();
            return JS_INVALID_REFERENCE;
        }

        tranquil::Timeout timeout(func, time, tranquil::Value(false));
        this->taskQueue.push(new Task(timeout.object, context, JS_INVALID_REFERENCE));
        
        return timeout.object;
    }

    JsValueRef EventLoop::SetInterval(bool isConstructor, tranquil::Value context, std::vector<tranquil::Value> args) {
        if (isConstructor || args.size() < 2)
            return JS_INVALID_REFERENCE;

        tranquil::Value func = args[0];
        tranquil::Value time = args[1];

        if (func.GetType() != JsFunction && time.GetType() != JsNumber) {
            tranquil::Exceptions::InvalidArgument();
            return JS_INVALID_REFERENCE;
        }

        tranquil::Timeout timeout(func, time, tranquil::Value(false));
        this->taskQueue.push(new Task(timeout.object, context, JS_INVALID_REFERENCE, true));
        
        return timeout.object;
    }

    JsValueRef EventLoop::ClearTimeout(bool isConstructor, tranquil::Value context, std::vector<tranquil::Value> args) {
        if (isConstructor || args.size() < 1)
            return JS_INVALID_REFERENCE;

        if (args[0].GetType() != JsObject) {
            tranquil::Exceptions::InvalidArgument();
            return JS_INVALID_REFERENCE;
        }

        Timeout timeout(args[0]);
        timeout.Destroy();

        return JS_INVALID_REFERENCE;
    }
}