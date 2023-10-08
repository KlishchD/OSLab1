#include <chrono>
#include <windows.h>
#include <fcntl.h>
#include <future>
#include <iostream>

#include "Manager.h"
#include "Utils.h"

void Manager::runFunction(float x, char function, std::promise<Result> &&promise) {
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

    if (word == WAIT_OBJECT_0) {
        Result result;
        std::string path = std::string(1, function) + "out.txt";
        readResult(path, result);
        promise.set_value(result);
    } else {
        promise.set_value({ 0.0f, 0, false });
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void Manager::readResult(const std::string& path, Result& result) {
    lock.lock();
    Utils::setUpInput(path);
    std::cin >> result;
    lock.unlock();
}

Result Manager::runFunctions(float x) {
    std::promise<Result> fPromise;
    std::future<Result> fFuture = fPromise.get_future();

    std::promise<Result> gPromise;
    std::future<Result> gFuture = gPromise.get_future();

    runFunction(x, 'f', fResults, std::move(fPromise));
    runFunction(x, 'g', gResults, std::move(gPromise));

    Result fResult = fFuture.get();
    Result gResult = gFuture.get();

    if (!fResults.count(x)) fResults[x] = fResult;
    if (!gResults.count(x)) gResults[x] = gResult;

    if (fResult.isSuccessful && gResult.isSuccessful) {
        return {fResult.value + gResult.value, fResult.nonCriticalErrorCount + gResult.nonCriticalErrorCount, true};
    }

    for (std::thread &thread: threads) {
        thread.join();
    }
    threads.clear();

    return {(float) !fResult.isSuccessful + 2 * !gResult.isSuccessful, 0, false};
}

void Manager::runFunction(float x, char function, std::map<float, Result>& table, std::promise<Result>&& promise) {
    if (table.count(x)) {
        promise.set_value(table[x]);
    } else {
        std::thread fFunction([this, x, function, &promise]() { runFunction(x, function, std::move(promise)); });
        threads.push_back(std::move(fFunction));
    }
}

void Manager::setTimeLimit(float inTimeLimit) {
    timeLimit = inTimeLimit;
}
