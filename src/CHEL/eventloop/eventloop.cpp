#include "eventloop.h"

#include "CHEL/common.h"

#include "CHEL/boolean.h"
#include "CHEL/object.h"
#include "CHEL/number.h"

namespace JS {
    void EventLoop::SetCallback() {
        if (JsSetPromiseContinuationCallback(PromiseCallback, this) != JsNoError)
            throw FatalRuntimeException();
    }

    void EventLoop::PromiseCallback(JsValueRef task, void* callbackState) {
        JsValueRef global = JS::Common::GetGlobalObject();
        auto* queue = static_cast<std::queue<Task*>*>(callbackState);

        JsValueRef jsObj = JS::Common::GetNewObject();
        JS::Object object(jsObj);
        object.SetProperty("_destroyed", JS::Boolean::ToJS(false));
        object.SetProperty("_onTimeout", task);

        queue->push(new Task(jsObj, global, JS_INVALID_REFERENCE, JS::Boolean::ToJS(false)));
    }

    void EventLoop::Loop() {
        while (!taskQueue.empty()) {
            Task* task = taskQueue.front();
            taskQueue.pop();
            auto timeout = JS::Timeout(task->timeout);
            int currentTime = (int)(clock() / (double)(CLOCKS_PER_SEC / 1000));
            if (!JS::Boolean::FromJS(timeout._destroyed)) {
                if (currentTime-task->time > JS::Number::FromJS(timeout._repeat)) {
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