#ifndef _GAMER_H_
#define _GAMER_H_

#include "Person.h"

#include <string>
#include <map>
#include <stdexcept>
#include <iostream>
#include <iomanip>

class NoScoreException: public exception{
    virtual const char* what() const throw(){
       return "No Score";
    }
};

class Gamer: public Person{
    public:
        Gamer(const int &regNo, string &name, float eAccumulative, float mAccumulative, float hAccumulative, float eAverage, float mAverage, float hAverage, int eGames, int mGames, int hGames);

	    //adds the score from the game the user just played
	    void addScore(const float &score,const int &difficulty);

	    //displays total score for each difficulty and throws no score expetion if there is no score
	    bool updateAccumulativeScore(const int &difficulty,const float &score);

        //calculates the new average amount after each game
        bool updateAverage(const int &difficulty);

        //increments (+1) number of games
        bool updateNoOfGames(const int &difficulty);

        //return accumulative score value for specified difficulty;
        float getAcummulativeScore(const int &difficulty);

        //controls how Gamer is printed
        friend ostream& operator<<(ostream &str, const Gamer &g);

        //allows me to compare Gamer objects
        bool operator==(const Gamer& other) const;

        string getInfo() const;

    private:
	    multimap<string,float> scores;
	    map<string,float> accumulative_scores;
	    map<string,float> average_scores;
	    int games[3]; //tracks number of games for each DIFFICULTY
	    float minMax[6]; //stores the min/max scores for the users games
};

#endif
