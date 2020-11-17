#ifndef UTILITY_FUNCTIONS_
#define UTILITY_FUNCTIONS_

#include<vector>
#include<string>

using namespace std;

//Split a string into a vector of integers,
vector<int> splitStringToIntVect(string s, string delimiters) {
    //Indexes on the string
    int beg, pos = 0;
    //Number that will be read from string
    int num;
    //Vector of numbers that will be read from string
    vector<int> numbers;
    //Starting at pos, find first not of delimiters repeatedly until it
    //gets to the end
    while((beg = s.find_first_not_of(delimiters, pos)) != string::npos) {
        //Find the next delimiter
        pos = s.find_first_of(delimiters, beg + 1);
        //The number will be the substring starting at beg of size
        //pos - beg
        num = stoi(s.substr(beg, pos - beg));
        //Push num to vector
        numbers.push_back(num);
    }
    return numbers;
 }

#endif
