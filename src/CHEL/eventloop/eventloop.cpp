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
        queue->push(new Task(task, 0, global, JS_INVALID_REFERENCE));
    }

    void EventLoop::Loop() {
        while (!taskQueue.empty()) {
            Task* task = taskQueue.front();
            taskQueue.pop();
            int currentTime = (int)(clock() / (double)(CLOCKS_PER_SEC / 1000));
            if (currentTime-task->time > task->delay) {
                task->Invoke();
                if (task->repeat) {
                    task->time = currentTime;
                    taskQueue.push(task);
                } else {
                    delete task;
                }
            } else {
                taskQueue.push(task);
            }
        }
    }
}