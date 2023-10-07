#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include "Result.h"

Result f(float x) {
    return { x * x, 0, true };
}

Result g(float x) {
    return { x, 0, true };
}

void setUpOutput(const std::string& path) {
    int out = open(path.c_str(), OF_WRITE | O_CREAT, 0777);
    dup2(out, STDOUT_FILENO);
    close(out);
}

int main(int argc, TCHAR *argv[]) {
    setUpOutput(std::string(argv[2]) + "out.txt");

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

/*
*/
