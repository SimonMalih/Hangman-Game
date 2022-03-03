#ifndef READWORDS_H
#define READWORDS_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <ctime>
#include <set>
#include <vector>
#include <map>

using namespace std;

 class ReadWords{
   public:
     ReadWords(const string &filename);

     ReadWords();

     //closes file
     void close();

     //read and store valid allSentences from the file
     void storeSentences();

     //store all words into vector with sentence location
     void storeWords();

     //get max length of the words
     int getMaxWordLength() const;

     //get min length of words
     int getMinWordLength() const;

     //fills words and phrases sets
     bool fillWordsAndPhrases(const int &min, const int &max);

     //add element to allWords if it doesnt exist else adds sentence position to exisiting sub vector<twoDimensional>
     void findElement(const string &inputWord, const int &position);

     const set<string>* getWordSet();
     const set<string>* getPhraseSet();
     const set<string>* getSimilarWords(string word, string SECRET);

     vector<string> sentenceSplitter(const string &sentence);

 private:
     class twoDimensional{ //stores word and position of sentence the word was found in
     public:
         twoDimensional(const string &words,  vector<int> vectors){
             word = words;
             vect = vectors;
         }
         string word;
         vector<int> vect;
         bool operator< (const twoDimensional &other) const {
             return word.length() < other.word.length();
         }
     };

     ifstream wordfile;
     int maxWordLength, minWordLength;
     set<string> words, phrases, similarWords;
     map<int,string>allSentences; //stores allSentences with associated number which will be used to decide what sentence words are from
     vector<twoDimensional>allWords;
 };
 #endif