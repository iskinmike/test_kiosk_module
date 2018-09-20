
#include <thread>
#include <string>
#include <cstring>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "wilton/wiltoncall.h"

#include "browser/browser.hpp"

namespace kiosk {


std::string run_browser(int proxy) {
    (void) proxy;
    start_browser();
    return std::string {"success"};
}

char* wrapper_run_browser(void* ctx, const char* data_in, int data_in_len, char** data_out, int* data_out_len) {
    try {
        auto fun = reinterpret_cast<std::string(*)(int)> (ctx);
        auto input = std::string(data_in, static_cast<size_t>(data_in_len));
        auto timeout = std::stoi(input);
        std::string output = fun(timeout);
        if (!output.empty()) {
            // nul termination here is required only for JavaScriptCore engine
            *data_out = wilton_alloc(static_cast<int>(output.length()) + 1);
            std::memcpy(*data_out, output.c_str(), output.length() + 1);
        } else {
            *data_out = nullptr;
        }
        *data_out_len = static_cast<int>(output.length());
        return nullptr;
    } catch (...) {
        auto what = std::string("CALL ERROR"); // std::string(e.what());
        char* err = wilton_alloc(static_cast<int>(what.length()) + 1);
        std::memcpy(err, what.c_str(), what.length() + 1);
        return err;
    }
}
} // namespace kiosk

extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
char* wilton_module_init() {
    char* err = nullptr;

    auto run_browser_name = std::string("run_browser");
    err = wiltoncall_register(run_browser_name.c_str(), static_cast<int> (run_browser_name.length()),
            reinterpret_cast<void*> (kiosk::run_browser), kiosk::wrapper_run_browser);
    if (nullptr != err) return err;

    // return success
    return nullptr;
}
