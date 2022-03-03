#include "Highlight.h"

///using ANSI escape code to highlight text

Highlight::Highlight() {
    ERROR = "\033[31m", DEFAULT = "\033[0m", MANAGER = "\033[95m", GAMER = "\033[34m", WORD = "\033[96m", SCORE = "\033[92m", WARNING = "\033[91m", SUCCESS = "\033[32m";
}

//highlight group of text
void Highlight::highlightText(const std::vector<Highlights> &sentences) {
    for(auto itr = sentences.begin(); itr != sentences.end(); ++itr)
        std::cout<< itr->colour + itr->sentence;
    std::cout<<DEFAULT<<std::endl;
}

//highlight sentence
void Highlight::highlightText(const std::string &sentence, const std::string &COLOUR) {
    std::cout<<COLOUR+sentence+DEFAULT<<std::endl;
}

//returns highlighted string
std::string Highlight::customH(const std::string &sentence, const std::string &COLOUR) {
    return COLOUR+sentence+DEFAULT;
}







