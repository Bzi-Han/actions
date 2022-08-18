#include "Finally.h"

#include "config.h"

#include <quickjs-cmake/quickjs/quickjs.h>
#include <quickjs-cmake/quickjs/quickjs-libc.h>

#include <iostream>

const char script[] = R"(
    import * as os from "os";

    let timers = [];
    for(let i = 0; i < 4; i++)
        timers[i] = os.setTimeout(() => print('[=] test timer'), 1000);
    for(let i = 0; i < 3; i++)
        os.clearTimeout(timers[i]);

    new Promise((resolve, reject) => {
        print('[=] test promise');
        os.setTimeout(() => resolve(), 1000);
    }).then(() => {
        print('[+] test promise resolve');
    }).catch(() => {
        print('[-] test promise catch');
    }).finally(() => {
        print('[=] test promise finally');
    });

    const test_async = async () => {
        print('[=] test async');
        new Promise((resolve, reject) => {
            reject({code: 400});
        }).catch((error) => print('[=] test async promise reject', error, error.code));
        print('[+] test async end');
    };

    const main = async () => {
        print('[=] main');
        await test_async();
        print('[=] main end');
    };

    main();
)";

int main()
{
    std::cout << "Version: " << VERSION << std::endl;

#if defined(_WIN32)
    std::cout << "Windows: hello world!" << std::endl;
#elif defined(__linux__)
    std::cout << "Linux: hello world!" << std::endl;
#elif defined(__APPLE__)
    std::cout << "Mac: hello world!" << std::endl;
#else
    std::cout << "Unknown: hello world!" << std::endl;
#endif

    auto jsRuntime = JS_NewRuntime();
    if (!jsRuntime)
    {
        std::cout << "Failed to create runtime" << std::endl;
        return 1;
    }
    auto jsContext = JS_NewContext(jsRuntime);
    if (!jsContext)
    {
        std::cout << "Failed to create context" << std::endl;
        return 1;
    }

    finally
    {
        js_std_free_handlers(jsRuntime);
        JS_FreeContext(jsContext);
        JS_FreeRuntime(jsRuntime);
    };

    js_std_add_helpers(jsContext, 0, nullptr);
    js_std_init_handlers(jsRuntime);
    js_init_module_os(jsContext, "os");

    auto jsResult = JS_Eval(jsContext, script, sizeof(script) - 1, "<input>", JS_EVAL_TYPE_MODULE);
    if (JS_IsException(jsResult))
    {
        js_std_dump_error(jsContext);
        return 1;
    }

    js_std_loop(jsContext);

    std::cout << "Done" << std::endl;

    getchar();

    return 0;
}