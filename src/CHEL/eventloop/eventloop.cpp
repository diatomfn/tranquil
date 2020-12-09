#include "eventloop.h"

#include "CHEL/common.h"

namespace JS {
    void EventLoop::SetCallback() {
        if (JsSetPromiseContinuationCallback(PromiseCallback, this) != JsNoError)
            throw FatalRuntimeException();
    }

    void EventLoop::SetLog(Output::Log* log) {
        this->outputLog = log;
    }

    void EventLoop::PromiseCallback(JsValueRef task, void* callbackState) {
        JsValueRef global = JS::Common::GetGlobalObject();
        auto* queue = static_cast<std::queue<Task*>*>(callbackState);

        JS::Timeout timeout(task, JS::Number(0.0), JS::Boolean(false));
        queue->push(new Task(timeout.object, global, JS_INVALID_REFERENCE, false));
    }

    void EventLoop::HandleException(JsValueRef except) {
        JsValueRef jsException;

        if (JsGetAndClearExceptionWithMetadata(&jsException) != JsNoError)
            throw FatalRuntimeException();

        JS::Object exceptionMeta(jsException);
        JS::Object exception(exceptionMeta.GetProperty("exception"));

        JS::Value lineValue = exceptionMeta.GetProperty("line");
        JS::Value colValue = exceptionMeta.GetProperty("column");
        JS::Value trace = exception.GetProperty("stack");

        int line = (int)JS::Number(lineValue);
        int col = (int)JS::Number(colValue);

        this->outputLog->Push(trace, Output::LogType::ERR, line, col);
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