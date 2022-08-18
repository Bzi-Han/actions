#include <iostream>

int main()
{
#if defined(_WIN32)
    std::cout << "Windows: hello world!" << std::endl;
#elif defined(__linux__)
    std::cout << "Linux: hello world!" << std::endl;
#elif defined(__APPLE__)
    std::cout << "Mac: hello world!" << std::endl;
#else
    std::cout << "Unknown: hello world!" << std::endl;
#endif

    return 0;
}