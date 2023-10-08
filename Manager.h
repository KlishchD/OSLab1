//
// Created by Dklishch on 10/8/2023.
//

#ifndef OSLAB1_MANAGER_H
#define OSLAB1_MANAGER_H

#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <map>
#include "Result.h"

class Manager {
public:
    Result runFunctions(float x);
    void setTimeLimit(float inTimeLimit);
private:
    std::mutex lock;

    float timeLimit;

    std::map<float, Result> fResults;
    std::map<float, Result> gResults;

    std::vector<std::thread> threads;

    void readResult(const std::string& path, Result& result);
    void runFunction(float x, char function, std::map<float, Result>& table, std::promise<Result>&& future);
    void runFunction(float x, char function, std::promise<Result>&& promise);
};


#endif //OSLAB1_MANAGER_H
