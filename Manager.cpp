#include <chrono>
#include <windows.h>
#include <fcntl.h>
#include <future>
#include <iostream>

#include "Manager.h"
#include "Utils.h"
#include "StatusCodes.h"

std::thread Manager::runFunctionPromise(float x, char function, std::map<float, Result>& table, std::promise<Result> &&promise) {
    std::thread thread([this, x, function, &table, &promise]() {
        if (table.count(x)) {
            promise.set_value(table[x]);
            return;
        }

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        std::string arg0 = "C:\\Users\\Dklishch\\Documents\\Algebra\\OSLab1\\cmake-build-debug\\functions.exe";
        std::string arg1 = std::to_string(x);
        std::string str = arg0 + " " + arg1 + " " + function;
        LPSTR args = const_cast<LPSTR>(str.c_str());

        if (!CreateProcess("functions.exe", args, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            printf("CreateProcess failed (%lu).\n", GetLastError());
            return;
        }

        DWORD word = WaitForSingleObject(pi.hProcess, timeLimit);

        Result result;
        if (word == WAIT_OBJECT_0) {
            std::string path = std::string(1, function) + "out.txt";
            readResult(path, result);
        } else {
            result = {0.0f, RESULT_STATUS_TIMEOUT};
        }

        promise.set_value(result);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    });

    return thread;
}

void Manager::readResult(const std::string& path, Result& result) {
    resultReadLock.lock();
    Utils::setUpInput(path);
    std::cin >> result;
    resultReadLock.unlock();
}

RunResults Manager::runFunctions(float x, const callback& fCallback, const callback& gCallback) {
    std::vector<std::thread> threads;

    Result fResult, gResult;
    threads.push_back(runFunction(x, 'f', fResults, fResult, fCallback));
    threads.push_back(runFunction(x, 'g', gResults, gResult, gCallback));

    for (std::thread &thread: threads) {
        thread.join();
    }

    int status = RESULT_STATUS_HARD_FAULT;
    if (fResult.status == RESULT_STATUS_OK && gResult.status == RESULT_STATUS_OK) {
        status = RESULT_STATUS_OK;
    } else if (fResult.status != RESULT_STATUS_OK && gResult.status != RESULT_STATUS_OK) {
        status = RESULT_FAILED_BOTH;
    } else if (fResult.status == RESULT_STATUS_OK) {
        status = RESULT_FAILED_G;
    } else if (gResult.status == RESULT_STATUS_OK) {
        status = RESULT_FAILED_F;
    }

    return { fResult.value + gResult.value, status, fResult, gResult };
}

void Manager::setTimeLimit(float inTimeLimit) {
    timeLimit = inTimeLimit;
}

std::thread Manager::runFunction(float x, char function, std::map<float, Result>& table, Result& result, const callback& callback) {
    std::thread run([x, function, &table, &result, this, &callback]() {
        int retry = -1;
        do {
            std::promise<Result> promise;
            std::future<Result> future = promise.get_future();
            runFunctionPromise(x, function, table, std::move(promise)).join();
            result = future.get();

            ++retry;
        } while(result.status != RESULT_STATUS_OK && result.status != RESULT_STATUS_HARD_FAULT && retry < maxRetryCount);

        result.retries = retry;

        table[x] = result;

        callback(function, result);

        return 0;
    });

    return run;
}
