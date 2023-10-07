#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include "Result.h"

#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <chrono>

std::mutex lock;

void setUpInput(const std::string& path) {
    int out = open(path.c_str(), OF_READ);
    _dup2(out, STDIN_FILENO);
    _flushall();
}

void readResult(const std::string& path, Result& result) {
    lock.lock();
    setUpInput(path);
    std::cin >> result;
    lock.unlock();
}

void runFunction(float x, char function, std::promise<Result> promise) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    std::string arg0 = "C:\\Users\\Dklishch\\Documents\\Algebra\\OSLab1\\cmake-build-debug\\functions.exe";
    std::string arg1 = std::to_string(x);
    std::string str = arg0 + " " + arg1 + " " + function;
    LPSTR args = const_cast<LPSTR>(str.c_str());

    // Start the child process.
    if( !CreateProcess( "functions.exe",                        // No module name (use command line)
                        args,                        // Command line
                        NULL,                        // Process handle not inheritable
                        NULL,                        // Thread handle not inheritable
                        FALSE,                       // Set handle inheritance to FALSE
                        0,                           // No creation flags
                        NULL,                        // Use parent's environment block
                        NULL,                        // Use parent's starting directory
                        &si,                         // Pointer to STARTUPINFO structure
                        &pi )                        // Pointer to PROCESS_INFORMATION structure
            )
    {
        printf( "CreateProcess failed (%lu).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    DWORD word = WaitForSingleObject( pi.hProcess, WAIT_IO_COMPLETION );

    if (word == 0) {
        Result result;
        std::string path = std::string(1, function) + "out.txt";
        readResult(path, result);
        promise.set_value(result);
    }

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

int main() {
    float x = rand() % 1000;

    std::promise<Result> fpromise;
    std::future<Result> ffuture = fpromise.get_future();
    std::thread fFunction(runFunction, x, 'f', std::move(fpromise));


    std::promise<Result> gpromise;
    std::future<Result> gfuture = gpromise.get_future();
    std::thread gFunction(runFunction, x, 'g', std::move(gpromise));

    Result fResult = ffuture.get();
    Result gResult = gfuture.get();

    std::cout << "x: " << x << std::endl;

    if (!fResult.isSuccessful) {
        std::cout << "f critically failed" << std::endl;
    } else {
        std::cout << "f: " << fResult << std::endl;
    }

    if (!gResult.isSuccessful) {
        std::cout << "f critically failed" << std::endl;
    } else {
        std::cout << "g: " << gResult << std::endl;
    }

    if (fResult.isSuccessful && gResult.isSuccessful) {
        std::cout << "result: " << fResult.value + gResult.value << std::endl;
    }

    fFunction.join();
    gFunction.join();
}