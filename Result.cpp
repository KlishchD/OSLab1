//
// Created by Dklishch on 10/7/2023.
//

#include "Result.h"
#include <string>
#include <sstream>

std::ostream &operator<<(std::ostream &ostream, const Result &result) {
    return ostream << result.value << "," << result.status;
}

std::istream &operator>>(std::istream &istream, Result &result) {
    std::string str;
    std::getline(istream, str);

    int mid = str.find_first_of(',');

    result.value = std::stof(str.substr(0, mid));
    result.status = std::stoi(str.substr(mid + 1));

    return istream;
}
