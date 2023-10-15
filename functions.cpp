#include <iostream>
#include <windows.h>
#include <thread>
#include "Result.h"
#include "Utils.h"
#include "StatusCodes.h"

Result f(float x) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(0.5s + std::chrono::seconds(rand() % 2));
    return { x * x, RESULT_STATUS_OK + (rand() % 2) };
}

Result g(float x) {
    return {x, RESULT_STATUS_OK };
}

int main(int argc, TCHAR *argv[]) {
    srand (time(NULL));

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