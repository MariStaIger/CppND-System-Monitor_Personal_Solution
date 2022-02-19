#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "system.h"
#include "format.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
    return Process::pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 

    // total_time = stol(allValues[13]) + stol(allValues[14])
    long total_time = LinuxParser::ActiveJiffies(Pid());

    //seconds = LinuxParser::UpTime() - stol(allValues[21])/sysconf(_SC_CLK_TCK)
    long systemUp_time = LinuxParser::UpTime();

    float seconds = systemUp_time - (LinuxParser::UpTime(Pid())/sysconf(_SC_CLK_TCK));

    float cpu_util = (total_time/sysconf(_SC_CLK_TCK))/(seconds);

    return cpu_util; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime() - LinuxParser::UpTime(Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return stol(ram_) > stol(a.ram_);
 }