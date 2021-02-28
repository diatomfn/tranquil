#include "eventloop.h"

#include "CHEL/common.h"

namespace JS {
    void EventLoop::InitPromiseCallback() {
        if (JsSetPromiseContinuationCallback(PromiseCallback, this) != JsNoError)
            throw FatalRuntimeException();
    }

    void EventLoop::PromiseCallback(JsValueRef task, void* callbackState) {
        JS::Value global = JS::Value::GetGlobalContext();
        auto* queue = static_cast<std::queue<Task*>*>(callbackState);

        JS::Timeout timeout(task, JS::Value(0.0), JS::Value(false));
        queue->push(new Task(timeout.object, global, JS_INVALID_REFERENCE, false));
    }

    void EventLoop::HandleException(JsValueRef except) {
        JsValueRef jsException;

        if (JsGetAndClearException(&jsException) != JsNoError)
            throw FatalRuntimeException();

        this->errorCallback(JS::Value(jsException));
    }

    void EventLoop::SetErrorCallback(std::function<void(JS::Value)> callback) {
        this->errorCallback = callback;
    }

    void EventLoop::Loop() {
        while (!taskQueue.empty()) {
            Task* task = taskQueue.front();
            taskQueue.pop();
            auto timeout = JS::Timeout(task->timeout);

            int currentTime = (int)(clock() / (CLOCKS_PER_SEC / (double)1000));
            if (!JS::Value(timeout._destroyed).AsBool()) {
                if (currentTime-task->time > (int)(JS::Value(timeout._repeat).AsDouble())) {
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
}