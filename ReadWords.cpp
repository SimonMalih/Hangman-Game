#include "ReadWords.h"


using namespace std;

bool validWord(const string &word){
    if(word.length()<2)
        return false;

    int letterCount = 0;
    //checking if word has an invalid character such as @ etc
    for(int i = 0; i<word.length(); ++i){
        if(isalpha(word[i]))
            ++letterCount;
        if(!isalpha(word[i]) && !ispunct(word[i]))
            return false;
    }
    if(letterCount<=0)
        return false;

    return true;
}

//returns a vector of words from a sentence
vector<string> ReadWords::sentenceSplitter(const string &sentence) {
    vector<string> tempWordCollection;
    string tempWord;
    const string splitChars = " .?!,:;\"*<>/\\|#~";

    for(int i = 0; i<sentence.length(); ++i){
        if(splitChars.find(sentence[i])!=string::npos){
            if(!tempWord.empty())
                tempWordCollection.push_back(tempWord);
            tempWord.clear();
        } else {
            tempWord+=sentence[i];
            if(i == sentence.length()-1){
                tempWordCollection.push_back(tempWord);
                tempWord.clear();
            }
        }
    }
    return tempWordCollection;
}

//format the allSentences by removing unnecessary spaces from text and punctuation at the beginning of allSentences
string removeAllExtraSpaces(string sentence){
    string newSentence;
    int i = 0;

    while(!isalpha(sentence[i]) && !isdigit(sentence[i])){
        ++i;
    }

    for(;i<sentence.length(); ++i){
        if(sentence[i] == ' ' && sentence[i+1] == ' '){
            int offset = 1;
            while(sentence[i+offset] == ' ')
                ++i;
        }
        newSentence+=sentence[i];
    }
    int end;
    //removing all spaces at the beginning and end of allSentences
    while (true){
        if(newSentence[0] == ' ' || ispunct(newSentence[0]))
            newSentence.erase(0, 1);
        end = newSentence.length() - 1;
        if(newSentence[end] == ' ')
            newSentence.erase(newSentence.length() - 1, 1);
        end = newSentence.length() - 1;

        if(newSentence[0] != ' ' && newSentence[end] != ' ')
            break;
    }

    return newSentence;
}

//removing punctuation at the start and end of the word
string removePunct(string word){
    while(true){
        if(ispunct(word[0]))
            word.erase(0,1);

        int end = word.length()-1;
        if(ispunct(word[end]))
            word.erase(end,1);

        end = word.length()-1;
        if(!ispunct(word[0]) && !ispunct(word[end]))
            break;
    }
    return word;
}

//returns true if other is similar to word (for similar words in hangman game)
bool similarWordCheck(const string &word, const string &other) {
    if(!word.compare(other))
        return false;
    for(int i = 0; i<word.length(); ++i)
        if((isalpha(word[i]) && word[i] != other[i]) || ((ispunct(word[i]) && word[i]!=other[i]) && word[i]!='_'))
            return false;
    return true;
}

//attempts to open file and initialise members
ReadWords::ReadWords(const string &filename){
    wordfile.open(filename);//open file
    if (!wordfile){
        cout << "Failed to open " << filename << endl;
        exit(1);
    }
    storeSentences();
    close();
    storeWords();
}

ReadWords::ReadWords() {}

//check for full !,?,. in the middle of the sentence
bool sentenceAllowed(string &s){
    const string endSentence = ".!?";
    for(int i = 0; i<s.length()-1; i++)
        if(endSentence.find(s[i])!=string::npos)
            return false;

    int space = 0;
    //checking if the sentence has enough spaces
    for(int i = 0; i<s.length(); ++i){
        if(s[i]=='*' || s[i]=='#')
            return false;
        if(s[i] ==' ')
            space++;
        if(space>0)
            break;
    }
    //if there is no space in the sentence false is returned
    if(space<1)
        return false;
    return true;
}

//stores all sentences in sentence map
void ReadWords::storeSentences() {
    string line, sentenceIn;
    const string endSentence = ".!?";
    vector<string> sentenceCollection;

    //storing sentences into map
    while (getline(wordfile, line)){
        for(int i = 0; i<line.length(); ++i){
            if(line[i]=='\n' || line[i]=='\t' || line[i]=='\r')
                continue;
            sentenceIn+=line[i];
            if(endSentence.find(line[i])!=string::npos){
                sentenceIn = removeAllExtraSpaces(sentenceIn);
                sentenceCollection.push_back(sentenceIn);
                sentenceIn.clear();
            }
        }
    }

    //moving valid sentences from map to all sentence vector
    int position = 0;
    for(auto itr = sentenceCollection.begin(); itr!=sentenceCollection.end(); ++itr){
        string sentence = removeAllExtraSpaces(*itr);
        if(sentenceAllowed(sentence)){
            allSentences.insert({position, sentence});
            ++position;
        }
    }
}

//add sentence position to existing word else adds new word with sentence position
void ReadWords::findElement(const string &inputWord, const int &position) {
    for(auto itr = allWords.begin(); itr != allWords.end(); ++itr){
        if(!itr->word.compare(inputWord)){
            itr->vect.push_back(position);
            return;
        }
    }
    allWords.push_back({inputWord, vector<int>{position}});
}

//stores word and sentencePosition from allSentences in word vector while also tracking smallest and biggest words
void ReadWords::storeWords() {
    maxWordLength = 0, minWordLength = 10000000;
    allWords.clear();
    int sentencePosition = 0;

    //split allSentences from sentence vector
    for(auto & sentence : allSentences){
        const vector<string> splitWords = sentenceSplitter(sentence.second);
        for(auto itr = splitWords.begin(); itr != splitWords.end(); ++itr){
            string word = removePunct(*itr);
            if(validWord(word)){
                std::transform(word.begin(), word.end(), word.begin(), ::tolower); //convert word to lower case
                findElement(word, sentencePosition);
            }
        }
        ++sentencePosition;
    }

    if(allWords.begin()==allWords.end()){
        std::cout<<"Default word file does not have enough words!\nExiting program!"<<std::endl;
        exit(1);
    }
    allWords.shrink_to_fit();
    std::sort(allWords.begin(), allWords.end());
    minWordLength = allWords.begin()->word.length();
    auto allWordsLastElement = allWords.end();
    --allWordsLastElement;
    maxWordLength = allWordsLastElement->word.length();
}

void ReadWords::close(){
    wordfile.close();
}

bool ReadWords::fillWordsAndPhrases(const int &min, const int &max) {
    int start = 0, end = 0, position = 0;
    bool foundMin = false;

    for(auto itr=allWords.begin(); itr != allWords.end(); ++itr) {
        if (itr->word.length() >= min && !foundMin) {
            start = position;
            foundMin = true;
        }
        if (itr->word.length() > max)
            break;
        else {
            end = position;
            position++;
        }
    }

    words.clear();
    phrases.clear();

        //inserting 5 random words with the required lengths and their phrases to the sets
        srand((unsigned) time(NULL));
        for(int i = 0; i<5; ++i){
            int randomNumber = start + (rand()%(end-start));
            auto word = allWords.begin();
            advance(word, randomNumber);
            words.insert(word->word);
            randomNumber = rand()%word->vect.size();
            phrases.insert(allSentences.at(word->vect.at(randomNumber)));
        }
        return true;
}

int ReadWords::getMaxWordLength() const {
    return maxWordLength;
}
int ReadWords::getMinWordLength() const {
    return minWordLength;
}
const set<string> *ReadWords::getWordSet() {
    return &words;
}
const set<string> *ReadWords::getPhraseSet(){
    return  &phrases;
}
//retrieve similar words to the guessed word
const set<string> *ReadWords::getSimilarWords(string word, string SECRET) {
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    std::transform(SECRET.begin(), SECRET.end(), SECRET.begin(), ::tolower);

    similarWords.clear();
    std::sort(allWords.begin(), allWords.end());

    for(auto itr = allWords.begin(); itr != allWords.end(); ++itr){
        if(itr->word.length()>word.length())
            break;
        if(itr->word.length() == word.length()){
            if(similarWordCheck(word, itr->word) && itr->word != SECRET)
                similarWords.insert(itr->word);
        }
    }
    return &similarWords;
}


