//
// Created by Dklishch on 10/8/2023.
//

#include "Utils.h"
#include <windows.h>
#include <fcntl.h>

void Utils::setUpInput(const std::string &path) {
    if (stdinNo == -1) stdinNo = _dup(STDIN_FILENO);
    int in = open(path.c_str(), OF_READ);
    setSTDIN(in);
    close(in);
}

void Utils::setSTDIN(int fno) {
    _dup2(fno, STDIN_FILENO);
    _flushall();
}

void Utils::resetInput() {
    setSTDIN(stdinNo);
}

void Utils::setUpOutput(const std::string &path) {
    if (stdoutNo == -1) stdoutNo = _dup(STDOUT_FILENO);
    int out = open(path.c_str(), OF_WRITE | O_CREAT, 0777);
    setSTDOUT(out);
    close(out);
}

void Utils::setSTDOUT(int fno) {
    dup2(fno, STDOUT_FILENO);
    _flushall();
}

void Utils::resetOutput() {
    setSTDOUT(stdoutNo);
}

bool Utils::isNumeric(const std::string &string) {
    bool hasSeenPoint = false;
    for (int i = string[0] == '-'; i < string.size(); ++i) {
        if (string[i] == '.') {
            if (hasSeenPoint) return false;
            hasSeenPoint = true;
        } else if (!isdigit(string[i])) return false;
    }
    return true;
}
