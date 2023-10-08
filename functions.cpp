#include <iostream>
#include <windows.h>
#include <thread>
#include "Result.h"
#include "Utils.h"

Result f(float x) {
    return { x * x, 0, true };
}

Result g(float x) {
    return { x, 0, true };
}

int main(int argc, TCHAR *argv[]) {
    Utils::setUpOutput(std::string(argv[2]) + "out.txt");

    float x = std::stof(argv[1]);

    Result result;
    if (argv[2][0] == 'f') {
        result = f(x);
    } else {
        result = g(x);
    }

    std::cout << result << std::endl;
    return 0;
}