//
// Created by Dklishch on 10/7/2023.
//

#ifndef OSLAB1_RESULT_H
#define OSLAB1_RESULT_H


#include <ostream>

struct Result {
    float value;
    int nonCriticalErrorCount;
    bool isSuccessful;

    friend std::ostream& operator<<(std::ostream& ostream, const Result& result);
    friend std::istream& operator>>(std::istream& istream, Result& result);
};


#endif //OSLAB1_RESULT_H
