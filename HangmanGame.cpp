#include "Gamer.h"
#include "ReadWords.h"

extern Highlight h;

class HangmanGame {
public:
    HangmanGame(Gamer* g, const int &difficulty, string &word, const string &phrase, ReadWords &readWords, const string* DIFICULTY){
        this->difficulty = difficulty;
        this->word = word;
        this->phrase = phrase;
        game(readWords, g, DIFICULTY);
    }

    //fills the blank with found letters word with blanks
    static void blankFiller(char guess, string& word, string& blank, int& lettersFound){
        for(int i = 0; i<word.length(); i++)
            if(word[i] == guess){
                blank[i] = guess;
                lettersFound++;
            }
    }

    void game(ReadWords &readWords, Gamer* g, const string* DIFICULTY){
        if(difficulty>2){
            h.highlightText("Could not play Hangman because an invalid difficulty was entered!",h.ERROR);
            return;
        }

        std::cout<<"Welcome to Hangman ("<<DIFICULTY[difficulty]<<")"<<std::endl;

        string blank, misses, input;
        int lives = 6, lettersFound = 0;
        char guess;
        const string hangman[] = {"    |====|\n    o    |     \n   /|\\   |\n   / \\   |\n==========\n",
                                  "    |====|\n    o    |     \n   /|\\   |\n   /     |\n==========\n",
                                  "    |====|\n    o    |     \n   /|\\   |\n         |\n==========\n",
                                  "    |====|\n    o    |     \n   /|    |\n         |\n==========\n",
                                  "    |====|\n    o    |     \n    |    |\n         |\n==========\n",
                                  "    |====|\n    o    |     \n         |\n         |\n==========\n",
                                  "    |====|\n         |     \n         |\n         |\n==========\n"
        };

        string lowerCasePhrase = phrase;
        std::transform(lowerCasePhrase.begin(), lowerCasePhrase.end(), lowerCasePhrase.begin(), ::tolower);

        int start = (int) lowerCasePhrase.find(word), end = start+word.length();
        string phraseA,phraseB;
        if(start==-1)
            h.highlightText("Phrase not available!",h.WARNING);
        else if (start==0)
            phraseB = phrase.substr(end, phrase.length());
        else
            phraseA = phrase.substr(0, start), phraseB = phrase.substr(end, phrase.length());

        transform(word.begin(), word.end(), word.begin(), ::toupper);


        for(int i = 0; i<word.length(); i++){
            if(isalpha(word[i]))
                blank+= '_';
            else
                blank+= word[i];
        }

        //hangman game which will break when user guesses word or has no lives left
        while (true){
            std::cout<<hangman[lives];
            std::cout<<"Word: ";
            h.highlightText(blank,h.WORD);

            h.highlightText(vector<Highlight::Highlights>{{"Phrase: \""+phraseA,h.DEFAULT}, {blank,h.WORD}, {phraseB+"\"",h.DEFAULT}});

            if(lives<3){
                auto similar = readWords.getSimilarWords(blank, word);
                std::cout<<"Similar words: ";
                int count = 0;
                for(auto itr = similar->begin(); itr!=similar->end() && count<5; ++itr, ++count){
                    std::cout<<"["<<h.customH(*itr,h.WORD)<<"], ";
                }
                std::cout<<std::endl;
            }

            while (true){
                std::cout<<"Guess:";
                std::getline(cin, input);
                guess = (char) toupper(input[0]);
                if(isalpha(guess)){
                    if(misses.find(guess)!=string::npos || blank.find(guess)!=string::npos){
                        h.highlightText("Already guessed!",h.WARNING);
                    } else
                        break;
                } else
                    h.highlightText("Invalid Input!", h.ERROR);
            }


            if(word.find(guess)<word.length()){ //checking if guess is a letter in the word
                blankFiller(guess, word,blank, lettersFound); //replaces '_' with guessed letter
            } else {
                lives--;
                if(misses.find(guess)>misses.length()){
                    misses+= guess;
                    misses+=',';
                }
            }
            h.highlightText(vector<Highlight::Highlights>{{"Misses: ",h.DEFAULT},{misses+"\n",h.WARNING}});

            if(lives<=0){
                h.highlightText("You lost!",h.ERROR);
                std::cout<<hangman[0];
                std::cout<<"Word: ";
                h.highlightText(word,h.WORD);
                break;
            } else if(!blank.compare(word)){
                h.highlightText("You won!",h.SUCCESS);
                std::cout<<hangman[lives];
                std::cout<<"Word: ";
                h.highlightText(word,h.WORD);
                break;
            }
        }

        h.highlightText(vector<Highlight::Highlights>{{"Score is ",h.DEFAULT}, {to_string(lives)+"\n",h.SCORE}});

        //checking if updating the gamer was successful
        if(!g->updateAccumulativeScore(difficulty, lives))
            h.highlightText("Failed to update the accumulative score!",h.ERROR);
        if(!g->updateNoOfGames(difficulty))
            h.highlightText("Failed to update the number of games played!",h.ERROR);
        if(!g->updateAverage(difficulty))
            h.highlightText("Failed to update the average games played!",h.ERROR);
        g->addScore(lives,difficulty);
    }

private:
    string word, phrase;
    int difficulty;
};