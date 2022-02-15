#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long h;
    long min;
    long sec;

    string h_output;
    string min_output;
    string sec_output;

    h = seconds/3600;
    min = (seconds / 60) % 60;
    sec = seconds % 60;

    if (h<10)
        h_output = "0" + to_string(h);
    else
        h_output = to_string(h);

    if (min<10)
        min_output = "0" + to_string(min);
    else
        min_output = to_string(min);

    if (sec<10)
        sec_output = "0" + to_string(sec);
    else
        sec_output = to_string(sec);

    return h_output + ":" + min_output + ":" + sec_output; 

}