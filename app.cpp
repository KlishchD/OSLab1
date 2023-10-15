#include <iostream>
#include <tchar.h>
#include "Utils.h"
#include "Result.h"
#include "Manager.h"
#include "StatusCodes.h"

int main(int argc, TCHAR* argv[]) {
    std::string str;
    Manager manager;

    manager.setTimeLimit(std::stof(argv[1]));

    std::mutex lock;
    Manager::callback callback = [&lock](char function, const Result& result) {
        lock.lock();

        if (result.status == RESULT_STATUS_OK) {
            std::cout << function << " succeed value is " << result.value << std::endl;
        } else if (result.status == RESULT_STATUS_SOFT_FAULT) {
            std::cout << function << " has had soft fault and was not been able to resolve it" << std::endl;
        } else if (result.status == RESULT_STATUS_HARD_FAULT) {
            std::cout << function << " has had hard fault" << std::endl;
        } else if (result.status == RESULT_STATUS_TIMEOUT) {
            std::cout << function << " was not able to finnish before timeout" << std::endl;
        }
        std::cout << "Had " << result.retries << " retries" << std::endl;

        lock.unlock();
    };

    while (true) {
        std::cout << "Type (q) to exit" << std::endl;
        std::cout << "Type number to run functions" << std::endl;
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty()) {
            if (Utils::isNumeric(s)) {
                float x = std::stof(s);

                RunResults result = manager.runFunctions(x, callback, callback);

                if (result.status == RESULT_STATUS_OK) {
                    std::cout << "Both succeeded and the result is " << result.value << std::endl;
                } else {
                    std::cout << "Cannot calculate final result as ";
                    if (result.status == RESULT_FAILED_BOTH) {
                        std::cout << "failed both";
                    } else if (result.status == RESULT_FAILED_F) {
                        std::cout << "failed f";
                    } else if (result.status == RESULT_FAILED_G) {
                        std::cout << "failed g";
                    }
                    std::cout << std::endl;
                }

                Utils::resetInput();
            } else if (s == "q") {
                break;
            }
        }
    }

    return 0;
}