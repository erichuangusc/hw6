#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <random>
#include <chrono>
#include <cctype>

using namespace std;

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }

    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& input_string) const {
        std::size_t char_values[input_string.length()];  // Stores numeric values of characters
        for (unsigned int i = 0; i < input_string.length(); i++) {
            char_values[i] = letterDigitToNumber(input_string[i]);
        }

        unsigned long long int base36_segments[5] = {0, 0, 0, 0, 0};
        int char_index = input_string.length() - 1;  
        int segment_index = 4;  

        while (char_index >= 0) {
            unsigned long long int digit1 = 0, digit2 = 0, digit3 = 0, digit4 = 0, digit5 = 0, digit6 = 0;

            if (char_index >= 0) digit6 = char_values[char_index--];
            if (char_index >= 0) digit5 = char_values[char_index--]; 
            if (char_index >= 0) digit4 = char_values[char_index--]; 
            if (char_index >= 0) digit3 = char_values[char_index--]; 
            if (char_index >= 0) digit2 = char_values[char_index--]; 
            if (char_index >= 0) digit1 = char_values[char_index--];  

            base36_segments[segment_index] = (pow(36, 5) * digit1) + 
                                            (pow(36, 4) * digit2) + 
                                            (pow(36, 3) * digit3) + 
                                            (pow(36, 2) * digit4) + 
                                            (36 * digit5) + 
                                            digit6;
            segment_index--;
        }

        unsigned long long int hash_value = rValues[0] * base36_segments[0];
        for (int i = 1; i < 5; i++) {
            hash_value += rValues[i] * base36_segments[i];
        }
        return hash_value;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const {
        if (isdigit(letter)) {
            return letter - '0' + 26; 
        } 
        else if (isalpha(letter)) {
            return tolower(letter) - 'a';
        }
        return 0;  
    }


    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i {0}; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
