#ifndef EXERCISE_1_HIGHLIGHT_H
#define EXERCISE_1_HIGHLIGHT_H



#include <iostream>
#include <string>
#include <map>
#include<cstdlib>
#include <vector>

class Highlight {
public:
    struct Highlights {
        std::string sentence;
        std::string colour;
    };

    std::string ERROR , DEFAULT, MANAGER , GAMER , WORD , SCORE , WARNING , SUCCESS;
    Highlight();
    void highlightText(const std::string &sentence, const std::string &COLOUR);
    void highlightText(const std::vector<Highlights> &sentences);
    std::string customH(const std::string &sentence, const std::string &COLOUR);
};

#endif //EXERCISE_1_HIGHLIGHT_H