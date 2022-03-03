#include "Gamer.h"
#include "MyList.cpp"
#include "Manager.h"
#include "ReadWords.h"
#include "HangmanGame.cpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <type_traits>

///PLEASE RUN IN LINUX OTHERWISE YOU WILL NOT GET HIGHLIGHTING!!!
///I changed some of the original header file functions because you said they were there just to guide us:)
///I must admit the program does what its meant to but maybe not in the way you wanted us to do it hope you enjoy Michael and not mark me too harshly:)

const string UserFile = "users.txt", DifficultyFile = "difficulty.txt", DefaultWord = "sample.txt", DIFFICULTY[3] = {"EASY", "MEDIUM", "HARD"};
int difficultySettings[3][2];

Manager currentManager = Manager(2000001,"Simon");
ReadWords readWords = ReadWords();
Highlight h = Highlight();

//stores managers and gamers
std::vector<Gamer> gamerCollection;
std::vector<Manager> managerCollection;

//can store gamer/manager info
struct UserInformation {
    int regNo, eGames, mGames, hGames;
    string name;
    float eAccumulative, mAccumulative, hAccumulative, eAverage, mAverage, hAverage;
};

//splits users in file into attributes and attempts to convert to appropriate data types
bool lineSplitter(string line, UserInformation *user, int &option){
    string info[11];
    int dashCounter = 0;

    for(int i = 0; i<line.length(); ++i){
        if(line[i]=='|'){
            dashCounter++;
            if(dashCounter==11)
                break;
        } else
            info[dashCounter] += line[i];
    }

    if(dashCounter==2){
       try{
           user->regNo = stoi(info[0]);
           user->name = info[1];
           option = 1;
       } catch (...){
           return false;
       }
    }else if(dashCounter==11){
        try{
            user->regNo = stoi(info[0]);
            user->name = info[1];
            user->eAccumulative = stof(info[2]);
            user->mAccumulative = stof(info[3]);
            user->hAccumulative = stof(info[4]);
            user->eAverage = stof(info[5]);
            user->mAverage = stof(info[6]);
            user->hAverage = stof(info[7]);
            user->eGames = stoi(info[8]);
            user->mGames = stoi(info[9]);
            user->hGames = stoi(info[10]);
            option = 2;
        } catch (...){
            return false;
        }
    }
    else{
        return false;
    }
    return true;
}

//reads all users in the file and update the gamer and manager collections
void readUsers(){
    ifstream read(UserFile);
    if(!read){
        h.highlightText("Unable to open user file!",h.ERROR);
        return;
    }

    gamerCollection.clear();
    managerCollection.clear();

    string line;
    int linePosition = 0, option = 0;
    UserInformation user;

    while (getline(read, line)){
        ++linePosition;
        if(line.find('|')==string::npos)
            continue;
        if(!lineSplitter(line, &user, option)){
            h.highlightText("INVALID USER HAS BEEN DETECTED ON LINE "+ to_string(linePosition)+" PLEASE CHECK USERS FILE!",h.ERROR);
            continue;
        }

        switch (option) {
            case 1:
                if(user.regNo < 2000001){
                    h.highlightText(vector<Highlight::Highlights>{{"Manager user ", h.ERROR}, {to_string(user.regNo), h.MANAGER}, {" is invalid!", h.ERROR} });
                    continue;
                }
                managerCollection.push_back(Manager(user.regNo, user.name));
                break;
            case 2:
                if(user.regNo > 2000000 || user.regNo < 1){
                    h.highlightText(vector<Highlight::Highlights>{{"Gamer user ", h.ERROR}, {to_string(user.regNo), h.GAMER}, {" is invalid!", h.ERROR}});
                    continue;
                }
                gamerCollection.push_back(Gamer(user.regNo, user.name, user.eAccumulative, user.mAccumulative, user.hAccumulative, user.eAverage, user.mAverage, user.hAverage, user.eGames, user.mGames, user.hGames));
                break;
            default:
                h.highlightText("INVALID USER HAS BEEN DETECTED PLEASE CHECK USERS FILE!",h.ERROR);
                continue;
        }
    }
    read.close();
}

//write user file using the gamers and managers info from the collections
void writeUsers(){
    ofstream write(UserFile);
    if(!write){
        h.highlightText("Could not open user file!\nUser file was not changed!",h.ERROR);
        return;
    }
    //write all managers/gamers from the collections to the user file
    for(auto itr = managerCollection.begin(); itr!=managerCollection.end(); ++itr)
        write<<itr->getInfo()<<std::endl;
    for(auto itr = gamerCollection.begin(); itr!=gamerCollection.end(); ++itr)
        write<<itr->getInfo()<<std::endl;
    write.close();
}

//append new user to user file
void updateUsers(const string &User){
    ofstream updateUserFile(UserFile, std::ios_base::app);;
    if(!updateUserFile){
        h.highlightText("Could not open user file!\nUser file was not changed!",h.ERROR);
        return;
    }
    updateUserFile << User << std::endl;
    updateUserFile.close();
}

//checks if word is in the phrase
bool findPhrase(const string &word, const string &phrase){
    vector<string> w = readWords.sentenceSplitter(phrase);
    for(auto itr = w.begin(); itr!=w.end(); ++itr)
        if(*itr==word)
            return true;
    return false;
}

//gets word and phrase
void setWordPhrase(const set<string>* words, const set<string>*phrases, string* outPut){
    for(auto wordItr = words->begin(); wordItr!=words->end(); ++wordItr){
        outPut[0] = *wordItr;
        for(auto phraseItr = phrases->begin(); phraseItr!=phrases->end(); ++phraseItr){
            string lowerCasePhrase = *phraseItr;
            if(findPhrase(outPut[0], *phraseItr)){
                outPut[1] = *phraseItr;
                return;
            }
        }
    }
}

//if the string is an invalid number -1 will be returned
int validNumberConvertor(const string &word){
    if(word.length()>10 || word.empty())
        return -1;
    for(int i = 0; i<word.length(); i++){
        if(!isdigit(word[i]))
            return -1;
    }
    return stoi(word);
}

//overloaded validNumberConvertor which also checks if the converted int is bigger than max
int validNumberConvertor(const string &word, const int &max){
    const int number = validNumberConvertor(word);
    return number<=max ? number:0;
}

int validRegNoConvertor(const string &regNo, const int &min, const int &max){
    int n = validNumberConvertor(regNo);
    if(n !=-1 && n >= min && n <= max)
        return n;
    else
        return -1;
}

//returns false if there are non Alphabet letters
bool validName(const string &word){
    if(word.length()>255|| word.empty() || word[0]==' ')
        return false;
    for(int i = 0; i<word.length(); i++)
        if(!isalpha(word[i]) && word[i]!=' ' || word[i]=='|')
            return false;
    return true;
}

//reads game levels from file and stores them
void readDifficulty(){
    ifstream read(DifficultyFile);

    if(!read){
        h.highlightText("Unable to open DIFFICULTY file!\nDifficulty file has not been reset!",h.ERROR);
        return;
    }

    string info;
    int diffSet[6];
    getline(read, info);

    //checking for invalid characters
    for(int i = 0; i<info.length(); i++){
        if(!isdigit(info[i]) && info[i]!='|'){
            h.highlightText("Difficulty file has invalid characters!\nDifficulty levels have not been set!",h.ERROR);
            return;
        }
    }

    getline(read, info);
    string temp;

    int dashCounter = 0;
    for(int i = 0; i<info.length(); ++i){
        if(dashCounter >= 6)
            break;

        if(info[i] == '|'){
            diffSet[dashCounter] = stoi(temp);
            ++dashCounter;
            temp.clear();
        } else
            temp+= info[i];
    }

    int position = 0;
    for(int i = 0; i<3; i++)
        for(int a = 0; a<2; ++a)
            difficultySettings[i][a] = diffSet[position++];
    read.close();
}

//change game level settings
void writeDifficulty(const int &min, const int &max){
    string input, output;
    string message[2] = {"MIN", "MAX"};
    //get min/max value for each difficulty
    for(int i = 0; i < 3; i++){
        for(int a = 0; a<2; ++a){
            while(true){
                std::cout << "Please enter "<<message[a]<<" value for (" << DIFFICULTY[i] << ")" << std::endl;
                std::getline(cin, input);
                int number = validNumberConvertor(input);
                //number is checked to ensure that it does not exceed the max length of words available
                if(number>0){
                    if(number<min)
                        h.highlightText("Number is too small!",h.WARNING);
                    else if(number>max)
                        h.highlightText("Number is too big!",h.WARNING);
                    else if(a==1 && number<difficultySettings[i][0]){
                        h.highlightText("Number is not bigger than minimum!",h.WARNING);
                    } else{
                        difficultySettings[i][a] = number;
                        output += input+"|";
                        break;
                    }
                } else
                    h.highlightText("Invalid input please try again!\n",h.ERROR);
            }
        }
    }

    ofstream write (DifficultyFile);
    if(write)
        write<<output;
    else
        h.highlightText("File could not be found!\nDifficulty changes were not saved", h.ERROR);
    write.close();
}

//change file that stores game levels
void setDifficultyFile(const Manager *m){
    string file;
    ifstream test;
    while (true){
        std::cout<<"Please enter file name"<<std::endl;
        std::getline(cin, file);
        test.open(file);
        if(test){
            test.close();
            readWords = ReadWords(file);
            h.highlightText(vector<Highlight::Highlights>{{"New word file has been set!\n",h.SUCCESS}, {"Please set new minimum and maximum values!", h.DEFAULT}});
            break;
        } else {
            h.highlightText("Could not find file: "+file,h.ERROR);
        }
    }
    writeDifficulty(readWords.getMinWordLength(), readWords.getMaxWordLength());
    readDifficulty();
    currentManager = *m;
}

//game difficulty selection
void difficultyMenu(int &option){
    string input;
    while (true){
        for(int a = 0; a<3; ++a)
            cout<<a+1<<") "<<DIFFICULTY[a]<<" -> words with size between "<<difficultySettings[a][0]<<"-"<<difficultySettings[a][1]<<endl;
        getline(cin, input);
        option = validNumberConvertor(input, 3);
        if(option>0)
            break;
        else
            h.highlightText("Invalid input please try again",h.ERROR);
    }
    --option;
}

//top 10 scores will be printed for each DIFFICULTY
void topTen(const Gamer g){
    //making a copy of gamer collection so the pointer to the gamer is not moved
    std::vector<Gamer> tempGamers = gamerCollection;
    for(int i = 0; i < 3; i++){
        //custom lambda function sort is used to sort gamers in descending accumulative scores order
        std::sort(tempGamers.begin(),tempGamers.end(), [i](Gamer &a, Gamer &b){
           return a.getAcummulativeScore(i) > b.getAcummulativeScore(i);
        });
        std::cout<<"\nTop 10 Scores ("<<DIFFICULTY[i]<<")"<<endl;

        int position = 0;
        for(auto itr = tempGamers.begin(); position<10 && itr!=tempGamers.end(); itr++){
            if(itr->getRegNo() == g.getRegNo())
                h.highlightText(to_string(itr->getRegNo())+" "+to_string(itr->getAcummulativeScore(i)), h.GAMER);
            else
                std::cout<<to_string(itr->getRegNo())<<" "<<to_string(itr->getAcummulativeScore(i))<<std::endl;
            ++position;
        }
        std::cout<<endl;
    }
}

string createName(){
    string name;
    while (true){
        std::cout<<"Please enter your name"<<std::endl;
        getline(cin, name);
        if(validName(name))
            break;
        else
            h.highlightText("Invalid name please try again!",h.ERROR);
    }
    return name;
}

void createGamer(const int &id){
    h.highlightText("The user does not exist!",h.WARNING);
    std::cout<<"Creating new user"<<std::endl;
    string name = createName();
    gamerCollection.push_back(Gamer(id,name,0,0,0,0,0,0,0,0,0));
    updateUsers(gamerCollection.back().getInfo());
}

void createManager(){
    string input;
    int regNo;

    while (true){
        std::cout<<"Please enter registration number for the new Manager"<<std::endl;
        getline(cin, input);
        regNo = validRegNoConvertor(input,2000001, 3000000);
        //checking or duplicate
        for(auto itr = managerCollection.begin(); itr!=managerCollection.end(); ++itr){
            if(itr.base()->getRegNo() == regNo){
                h.highlightText("Manager already exists!",h.ERROR);
                regNo = -1;
            }
        }
        if(regNo!=-1)
            break;
        else
            h.highlightText("Invalid registration number please try again!!",h.ERROR);
    }

    string name = createName();
    h.highlightText(vector<Highlight::Highlights>{{"Manager ",h.SUCCESS}, {name, h.MANAGER}, {" (",h.SUCCESS}, {to_string(regNo), h.MANAGER}, {") was successfully created!",h.SUCCESS}});
    managerCollection.push_back(Manager(regNo, name));
    updateUsers(managerCollection.back().getInfo());
}

void managerMenu(Manager* m){
    string mName = m->getName(), mId = to_string(m->getRegNo());
    string input, name;
    int option;

    h.highlightText(vector<Highlight::Highlights>{{"Welcome ", h.DEFAULT}, {m->getName(), h.MANAGER}, {" (",h.DEFAULT}, {to_string(m->getRegNo()), h.MANAGER}, {")!", h.DEFAULT}});

    do{
        std::cout<<"1) Reset game\n2) Change game levels\n3) Add another manager\n4) Set the word file \n5) Change name\n6) Exit game"<<endl;
        getline(cin, input);
        option = validNumberConvertor(input,6);

        switch (option) {
            case 1: {
                ofstream defaultFileOpener("difficulty.txt");
                if (!defaultFileOpener) {
                    h.highlightText("The default file could not be found!\nTerminating program!\n", h.ERROR);
                    exit(1);
                }

                defaultFileOpener<<"2|3|4|7|8|10|";
                defaultFileOpener.close();
                readWords = ReadWords(DefaultWord);
                readDifficulty();
                defaultFileOpener.close();
                h.highlightText("Game was successfully reset!",h.SUCCESS);
            }
                break;
            case 2:
                std::cout<<"Current game levels"<<endl;
                for(int a = 0; a<3; ++a)
                    std::cout<<DIFFICULTY[a]<<" "<<to_string(difficultySettings[a][0])<<" -> "<<to_string(difficultySettings[a][1])<<endl;
                writeDifficulty(readWords.getMinWordLength(), readWords.getMaxWordLength());
                readDifficulty();
                currentManager = *m;
                h.highlightText(vector<Highlight::Highlights>{{"\nThe current manager is ", h.DEFAULT}, {currentManager.getName(), h.MANAGER}, {" (",h.DEFAULT}, {to_string(currentManager.getRegNo()), h.MANAGER}, {")", h.DEFAULT}});
                break;
            case 3:
                createManager();
                break;
            case 4:
                setDifficultyFile(m);
                currentManager = *m;
                h.highlightText(vector<Highlight::Highlights>{{"\nThe current manager is ", h.DEFAULT}, {currentManager.getName(), h.MANAGER}, {" (",h.DEFAULT}, {to_string(currentManager.getRegNo()), h.MANAGER}, {")", h.DEFAULT}});
                break;
            case 5:
                m->changeName(createName());
                h.highlightText("Name change was successful",h.SUCCESS);
                writeUsers();
                break;
            case 6:
                h.highlightText(vector<Highlight::Highlights>{{"Goodbye ", h.DEFAULT}, {mName, h.MANAGER}, {"!", h.DEFAULT}});
                break;
            default:
                h.highlightText("Invalid option please try again!",h.ERROR);
        }
    } while (option != 6);
}

void gamerMenu(Gamer* g){
    string input;
    int option = 0, difficulty = 0;

    h.highlightText(vector<Highlight::Highlights>{{"\nWelcome ", h.DEFAULT}, {g->getName(), h.GAMER}, {" (", h.DEFAULT}, {to_string(g->getRegNo()), h.GAMER}, {")!", h.DEFAULT} });
    string WordandPhrase[2];

    do{
        std::cout<<"1) Play game\n2) Your statistics\n3) Display top 10\n4) Change name\n5) Exit game"<<endl;
        getline(cin, input);
        option = validNumberConvertor(input,5);

        switch (option) {
            case 1: {
                difficultyMenu(difficulty);
                readWords.fillWordsAndPhrases(difficultySettings[difficulty][0], difficultySettings[difficulty][1]);
                setWordPhrase(readWords.getWordSet(), readWords.getPhraseSet(), WordandPhrase);
                HangmanGame(g, difficulty, WordandPhrase[0], WordandPhrase[1], readWords, DIFFICULTY);
                writeUsers();
                break;
            }
            case 2:
                std::cout<<*g<<std::endl;
                break;
            case 3:
                topTen(*g);
                break;
            case 4:
                g->changeName(createName());
                h.highlightText("Name change was successful",h.SUCCESS);
                writeUsers();
                break;
            case 5:
                h.highlightText(vector<Highlight::Highlights>{{"Goodbye ", h.DEFAULT}, {g->getName(), h.GAMER}, {" (", h.DEFAULT}, {to_string(g->getRegNo()), h.GAMER}, {")!\n", h.DEFAULT}});
                break;
            default:
                h.highlightText("Invalid input please try again!",h.ERROR);
        }
    } while(option != 5);
}

template <class T>
void login(T user){
    if (is_same<T,Gamer*>::value){
        gamerMenu((Gamer*)user);
    }else if (is_same<T,Manager*>::value){
        managerMenu((Manager*)user);
    }else
        h.highlightText("Invalid user object has been entered!\n",h.ERROR);
}

//attempts to find user and log them in with template login function
void findUser(const string &registrationNumber){
    ifstream read(UserFile);
    string line;
    int regNo = validRegNoConvertor(registrationNumber,1,3000000), userChoice;
    bool userExist = false;

    readUsers();

    //checking if the input is invalid
    if(regNo==-1){
        h.highlightText("Invalid registration number please try again!",h.ERROR);
        return;
    }

    //checks if the user exists by reading the ids and will turn userExist to true
    while (getline(read,line)){
        if(line.substr(0,line.find('|')) == registrationNumber){
            userExist = true;
            break;
        }
    }

    //if the user created is a manager and doesnt exist
    if(!userExist && regNo>2000000)
        h.highlightText("No manager with the provided registration number exists please try again!\n",h.ERROR);

    //logging in the Manager/Gamer object from the collection. If the gamer doesnt exist a new one will be created and logged in
    if(regNo>2000000){
        for(auto itr = managerCollection.begin(); itr!=managerCollection.end(); ++itr)
            if(itr->getRegNo()==regNo)
                login(itr.base());
    } else {
        for (auto itr = gamerCollection.begin(); itr!=gamerCollection.end(); ++itr){
            if (itr->getRegNo() == regNo){
                login(itr.base());
                return;
            }
        }

        //if the gamer doesnt exist a new one will be created and logged in
        createGamer(regNo);
        auto gamerItr = gamerCollection.end();
        --gamerItr;
        login(gamerItr.base());
    }
}

//starts the program in a default state. Default text file, default difficulty settings ensures that previous difficulties wont affect the operation
void runDefault(){
    //SetConsoleTextAttribute(color, 15);
    ofstream defaultFileOpener(DifficultyFile);
    if(!defaultFileOpener){
        h.highlightText("The default file could not be found!\nTerminating program!\n",h.ERROR);
        exit(1);
    }

    defaultFileOpener<<"2|3|4|7|8|10|";
    defaultFileOpener.close();

    readWords = ReadWords(DefaultWord);

    readDifficulty();
    readUsers();

    std::sort(managerCollection.begin(),managerCollection.end(), [](Manager &a, Manager &b){ //custom lambda function sort is used to sort gamers in descending accumulative scores order
        return a.getRegNo()<b.getRegNo();
    });

    currentManager = managerCollection.front();
}

//1-2000000 : Gamers
//2000001-3000000 : Managers

int main() {
    runDefault();
    h.highlightText(vector<Highlight::Highlights>{{"The current manager is ", h.DEFAULT}, {currentManager.getName(), h.MANAGER}, {" (",h.DEFAULT}, {to_string(currentManager.getRegNo()), h.MANAGER}, {")", h.DEFAULT}});

    string input;
    while (true){
        std::cout<<("Please log in using your registration number!\nEnter 0 to exit!")<<std::endl;
        getline(cin, input);
        if(input=="0")
            break;
        else
            findUser(input);
    }

    h.highlightText("The game has successfully terminated!", h.SUCCESS);
    writeUsers();
    return 0;
}