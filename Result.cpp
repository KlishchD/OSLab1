//
// Created by Dklishch on 10/7/2023.
//

#include "Result.h"
#include <string>
#include <sstream>

std::ostream &operator<<(std::ostream &ostream, const Result &result) {
    return ostream << result.value << "," << result.nonCriticalErrorCount << "," << result.isSuccessful;
}

std::istream &operator>>(std::istream &istream, Result &result) {
    std::string str;
    std::getline(istream, str);

    int mid1 = str.find_first_of(',');
    int mid2 = str.find_first_of(',', mid1 + 1);

    result.value = std::stof(str.substr(0, mid1));
    result.nonCriticalErrorCount = std::stoi(str.substr(mid1 + 1, mid2 - mid1 - 1));
    result.isSuccessful = std::stoi(str.substr(mid2 + 1));

    return istream;
}
