#ifdef _DEBUG
#include <iostream>
#include <intrin.h>
#define THROW_ERROR(error) std::cout << error << std::endl; __debugbreak();
#else
#define THROW_ERROR(error) // TODO: Make proper handling of error on release mode
#endif