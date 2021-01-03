#include "eventloop.h"

#include "CHEL/common.h"

namespace JS {
    void EventLoop::InitPromiseCallback() {
        if (JsSetPromiseContinuationCallback(PromiseCallback, this) != JsNoError)
            throw FatalRuntimeException();
    }

    void EventLoop::PromiseCallback(JsValueRef task, void* callbackState) {
        JS::Object global = JS::Object::GetGlobalObject();
        auto* queue = static_cast<std::queue<Task*>*>(callbackState);

        JS::Timeout timeout(task, JS::Number(0.0), JS::Boolean(false));
        queue->push(new Task(timeout.object, global, JS_INVALID_REFERENCE, false));
    }

    void EventLoop::HandleException(JsValueRef except) {
        JsValueRef jsException;

        if (JsGetAndClearException(&jsException) != JsNoError)
            throw FatalRuntimeException();

        this->errorCallback(JS::Object(jsException));
    }

    void EventLoop::SetErrorCallback(std::function<void(JS::Object)> callback) {
        this->errorCallback = callback;
    }

    void EventLoop::Loop() {
        while (!taskQueue.empty()) {
            Task* task = taskQueue.front();
            taskQueue.pop();
            auto timeout = JS::Timeout(task->timeout);

            int currentTime = (int)(clock() / (CLOCKS_PER_SEC / (double)1000));
            if (!JS::Boolean(timeout._destroyed)) {
                if (currentTime-task->time > (int)((double)JS::Number(timeout._repeat))) {
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