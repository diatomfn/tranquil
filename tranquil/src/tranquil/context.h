#pragma once

namespace tranquil {
    class Runtime;
    class Native;

    /**
     * @brief The Javascript execution context.
     * Each context contains its own global object.
     * Contexts cannot be transferred across runtimes.
     * Once a context is given to a Runtime it is initialized for that runtime.
     */
    class Context {
    private:
        uint contextNumber = 0;
        JsContextRef context = nullptr;

        friend Runtime;
        friend Native;
    };
}