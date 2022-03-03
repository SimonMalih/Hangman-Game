#include "Gamer.h"
const string DIFFICULTY[3] = {"EASY", "MEDIUM", "HARD"};

extern Highlight h;

//initialise all members
Gamer::Gamer(const int &regNo, string &name,  float eAccumulative, float mAccumulative, float hAccumulative, float eAverage, float mAverage, float hAverage, int eGames, int mGames, int hGames) : Person(name, regNo) {
    accumulative_scores.insert({DIFFICULTY[0],eAccumulative});
    accumulative_scores.insert({DIFFICULTY[1],mAccumulative});
    accumulative_scores.insert({DIFFICULTY[2],hAccumulative});
    average_scores.insert({DIFFICULTY[0],eAverage});
    average_scores.insert({DIFFICULTY[1],mAverage});
    average_scores.insert({DIFFICULTY[2],hAverage});
    games[0] = eGames;
    games[1] = mGames;
    games[2] = hGames;

    for(int i = 0; i<6; i++)
        minMax[i] = -1;
}

//addScore to the map while also updating min/max value
void Gamer::addScore(const float &score, const int &difficulty) {
    int position;
    if(difficulty == 0)
        position = 0;
    else if(difficulty==1)
        position = 2;
    else
        position = 4;

    try{
        scores.insert({DIFFICULTY[difficulty],score});
        if(minMax[position]==-1 || minMax[position+1]==-1){
            minMax[position] = score;
            minMax[position+1] = score;
        } else {
            minMax[position] = score<minMax[position] ? score : minMax[position];
            minMax[position+1] = score>minMax[position+1] ? score : minMax[position+1];
        }
    } catch (...){
        std::cout<<"Failed to add to the score!"<<std::endl;
    }
}

//update accumulative score by adding new score
bool Gamer::updateAccumulativeScore(const int &difficulty, const float &score){
    if(difficulty>2)
        return false;
    try{
        accumulative_scores.at(DIFFICULTY[difficulty]) +=score;
        return true;
    } catch(...){
        return false;
    }
}

//Increment total games played
bool Gamer::updateNoOfGames(const int &difficulty) {
    if(difficulty>2)
        return false;
    else
        games[difficulty]++;
    return true;
}

//update the average score for the determined DIFFICULTY
bool Gamer::updateAverage(const int &difficulty) {
    if(difficulty>2)
        return false;
    else {
        try{
            average_scores[DIFFICULTY[difficulty]] = accumulative_scores.at(DIFFICULTY[difficulty])/(float)games[difficulty];
            return true;
        } catch(...){
            return false;
        }
    }
}

float Gamer::getAcummulativeScore(const int &difficulty) {
    return accumulative_scores.at(DIFFICULTY[difficulty]);
}

//return name, regNo and scores for each difficulty
std::ostream& operator<<(ostream &str, const Gamer &g){
    string minMaxString[6];

    //if minMax is -1 no score exception will be thrown which will assign the score string to "No score"
    for(int i = 0; i<6; ++i){
        try {
            if(g.minMax[i]==-1){
                NoScoreException m;
                throw m;
            }
            minMaxString[i] = to_string(g.minMax[i]);
        } catch (exception &m){
            minMaxString[i] = m.what();
        }
    }

    str <<"\n"<<h.customH(g.getName(), h.GAMER) << " (" << h.customH(to_string(g.getRegNo()),h.GAMER) << ")" << std::endl;
    //values will be printed and shifted to the left with equal spacing5
    int width = 13;
    str<<left<<setw(width)<<"Difficulty"<<setw(width)<<"Accumulated"<<setw(width)<<"Average"<<setw(width)<<"Min"<<setw(width)<<"Max"<<setw(width)<<endl;
    str<<left<<setw(width)<<DIFFICULTY[0]<<setw(width)<<g.accumulative_scores.at(DIFFICULTY[0])<<setw(width)<<g.average_scores.at(DIFFICULTY[0])<<setw(width)<<minMaxString[0]<<setw(width)<<minMaxString[1]<<setw(width)<<endl;//<<setw(width)<<DIFFICULTY[0]<<setw(width)<<g.accumulative_scores.at(DIFFICULTY[0])<<setw(width)<<g.average_scores.at(DIFFICULTY[0])<<setw(width)<<minMaxString[0]<<setw(width)<<minMaxString[1]<<std::endl;;
    str<<left<<setw(width)<<DIFFICULTY[1]<<setw(width)<<g.accumulative_scores.at(DIFFICULTY[1])<<setw(width)<<g.average_scores.at(DIFFICULTY[1])<<setw(width)<<minMaxString[2]<<setw(width)<<minMaxString[3]<<setw(width)<<endl;//<<setw(width)<<DIFFICULTY[0]<<setw(width)<<g.accumulative_scores.at(DIFFICULTY[0])<<setw(width)<<g.average_scores.at(DIFFICULTY[0])<<setw(width)<<minMaxString[0]<<setw(width)<<minMaxString[1]<<std::endl;;
    str<<left<<setw(width)<<DIFFICULTY[2]<<setw(width)<<g.accumulative_scores.at(DIFFICULTY[2])<<setw(width)<<g.average_scores.at(DIFFICULTY[2])<<setw(width)<<minMaxString[4]<<setw(width)<<minMaxString[5]<<setw(width)<<endl;
    return str;
}

//gamers are equal when their regNo is the same
bool Gamer::operator==(const Gamer &other) const{
    if(this->regNo == other.regNo)
        return true;
    else
        return false;
}

//override getInfo to get gamer attributes
string Gamer::getInfo() const {
    float easyScore = 0, mediumScore = 0, hardScore = 0;

    //getting the values from the multimap for each DIFFICULTY
    for(auto itr = scores.begin(); itr!=scores.end(); itr++){
        string key = itr->first;
        float value = itr->second;

        if(!key.compare(DIFFICULTY[0]))
            easyScore+= value;
        else if(!key.compare(DIFFICULTY[1]))
            mediumScore+= value;
        else
            hardScore+= value;
    }

    string info = to_string(regNo)+"|"+name+"|";
    string accumulativeScores =  to_string(accumulative_scores.at(DIFFICULTY[0]))+"|"+to_string(accumulative_scores.at(DIFFICULTY[1]))+"|"+to_string(accumulative_scores.at(DIFFICULTY[2]))+"|";
    string averageScores = to_string(average_scores.at(DIFFICULTY[0]))+"|"+to_string(average_scores.at(DIFFICULTY[1]))+"|"+to_string(average_scores.at(DIFFICULTY[2]))+"|";
    string game = to_string(games[0]) + "|" + to_string(games[1]) + "|" + to_string(games[2]) + "|";
    return info + accumulativeScores + averageScores + game;
}