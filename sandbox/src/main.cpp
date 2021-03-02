#include <tranquil.h>

#include <iostream>

tranquil::Value BasicLog(bool isConstructor, tranquil::Value context, std::vector<tranquil::Value> args, void* callbackState);

tranquil::Value BasicLog(bool isConstructor, tranquil::Value context, std::vector<tranquil::Value> args, void* callbackState) {
    std::cout << args[0].AsString() << std::endl;
    return nullptr;
}

int main() {
    tranquil::Context context;
    tranquil::Runtime runtime(10, context);

    tranquil::Value global = tranquil::Value::GetGlobalObject();

    tranquil::Value log(ADAPT_FN(BasicLog), nullptr);
    global.SetProperty("log", log);
    
    runtime.SetErrorCallback([] (tranquil::Value error) {
        std::cout << error.AsString() << std::endl;
    });

    while (true) {
        std::cout << ">> ";

        std::string input;
        getline(std::cin, input);

        tranquil::Value result = runtime.Run("console", input);
        if (result != nullptr) {
            std::cout << result.AsString() << std::endl;
        }
    }
}
