//
// Created by Dklishch on 10/8/2023.
//

#ifndef OSLAB1_UTILS_H
#define OSLAB1_UTILS_H


#include <string>

class Utils {
public:
    static void setUpInput(const std::string& path);
    static void resetInput();

    static void setUpOutput(const std::string& path);
    static void resetOutput();

    static bool isNumeric(const std::string& string);

private:
    static inline int stdinNo = -1;
    static inline int stdoutNo = -1;

    static void setSTDIN(int fno);
    static void setSTDOUT(int fno);
};


#endif //OSLAB1_UTILS_H
