#include <iostream>
#include <tchar.h>
#include "Utils.h"
#include "Result.h"
#include "Manager.h"

int main(int argc, TCHAR* argv[]) {
    std::string str;
    Manager manager;

    manager.setTimeLimit(std::stof(argv[1]));

    while (true) {
        std::cout << "Type (q) to exit" << std::endl;
        std::cout << "Type number to run functions" << std::endl;
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty()) {
            if (Utils::isNumeric(s)) {
                float x = std::stof(s);
                Result result = manager.runFunctions(x);

                if (result.isSuccessful) {
                    std::cout << "Execution was successful" << std::endl;
                    std::cout << "Value is " << result.value << std::endl;
                    std::cout << "Count of noncritical errors is " << result.nonCriticalErrorCount << std::endl;
                } else {
                    std::cout << "Execution was not successful" << std::endl;
                    if ((int) result.value & 1) {
                        std::cout << "f has failed" << std::endl;
                    }
                    if ((int) result.value & 2) {
                        std::cout << "g has failed" << std::endl;
                    }
                }

                Utils::resetInput();
            } else if (s == "q") {
                break;
            }
        }
    }
}