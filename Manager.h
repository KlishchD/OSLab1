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

struct RunResults {
    float value;
    int status;
    Result fResult;
    Result gResult;
};

class Manager {
public:
    using callback = std::function<void(char, const Result&)>;
    RunResults runFunctions(float x, const callback& fCallback, const callback& gCallback);
    void setTimeLimit(float inTimeLimit);
private:
    std::mutex resultReadLock;
    std::mutex outputLock;

    float timeLimit;
    int maxRetryCount = 10;

    std::map<float, Result> fResults;
    std::map<float, Result> gResults;


    void readResult(const std::string& path, Result& result);
    std::thread runFunctionPromise(float x, char function, std::map<float, Result>& table, std::promise<Result>&& promise);
    std::thread runFunction(float x, char function, std::map<float, Result>& table, Result& result, const callback& callback);
};


#endif //OSLAB1_MANAGER_H
