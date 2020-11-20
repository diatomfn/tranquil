#include "eventloop.h"

#include "CHEL/common.h"

namespace JS {
    void EventLoop::SetCallback() {
        if (JsSetPromiseContinuationCallback(PromiseCallback, this) != JsNoError)
            throw FatalRuntimeException();
    }

    void EventLoop::PromiseCallback(JsValueRef task, void* callbackState) {
        JsValueRef global = JS::Common::GetGlobalObject();
        auto* queue = static_cast<std::queue<Task*>*>(callbackState);

        JS::Timeout timeout(task, JS::Number(0.0), JS::Boolean(false));
        queue->push(new Task(timeout.object, global, JS_INVALID_REFERENCE, false));
    }

    void EventLoop::Loop() {
        while (!taskQueue.empty()) {
            Task* task = taskQueue.front();
            taskQueue.pop();
            auto timeout = JS::Timeout(task->timeout);

            int currentTime = (int)(clock() / (CLOCKS_PER_SEC / (double)1000));
            if (!JS::Boolean(timeout._destroyed)) {
                if (currentTime-task->time > (int)((double)JS::Number(timeout._repeat))) {
                    task->Invoke();
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