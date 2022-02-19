#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  string unit;
  float memTotal;
  float memFree;
  float percentage;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && key != filterMemFreeString) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value >> unit;
      if(key == filterMemTotalString){
        memTotal = std::stof(value);
      } else if(key == filterMemFreeString){
        memFree = std::stof(value);
      }
    }
  }
  percentage = ((memTotal - memFree)/memTotal);
  return percentage;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string value1;
  string value2;
  long upTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value1 >> value2;
    upTime = std::stol(value1);
  }
  return upTime;

}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long totalJif = 0;
  int Jif;
  vector<string> values = LinuxParser::CpuUtilization();
  for(std::size_t i = 1; i<values.size(); i++){
    Jif = stoi(values[i]);
    totalJif = totalJif + Jif;
  }
  return totalJif; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  
  string line;
  string value;
  vector<string> allValues;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){;
      allValues.push_back(value);
    } 
  }
  //time from children processes is not included in total_time
  long total_time = stol(allValues[13]) + stol(allValues[14]);
  return total_time; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> allJiff =  LinuxParser::CpuUtilization();
  return stol(allJiff[1]) + stol(allJiff[2]) + stol(allJiff[3]) + stol(allJiff[6]) + stol(allJiff[7]) + stol(allJiff[8]) + stol(allJiff[9]) + stol(allJiff[10]); 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> allJiff =  LinuxParser::CpuUtilization();
  return stol(allJiff[4]) + stol(allJiff[5]); 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> values;
  string value, line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      values.push_back(value);
    }
  }

  return values; 
  
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int procsTotal;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && key != filterProcesses) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == filterProcesses){
        procsTotal = std::stoi(value);
      } 
    }
  }
  return procsTotal;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int procsRunning;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && key != filterRunningProcesses) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == filterRunningProcesses){
        procsRunning = std::stoi(value);
      } 
    }
  }
  return procsRunning;
  
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
   std::getline(filestream, line);
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string line;
  string key;
  string value;
  long valuel;
  string ram;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && key != filterProcMem) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == filterProcMem){
        valuel = stol(value) / 1024;
      } 
    }
  }
  ram = to_string(valuel);
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  string uid;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && key != filterUID) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == filterUID){
        uid = value;
      } 
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  
  string line;
  string user;
  string x;
  string uid;
  string user_output;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid;
      if(uid == LinuxParser::Uid(pid)){
         user_output = user;
      } 
    }
  }
return user_output;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  
  string line;
  string value;
  vector<string> allValues;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      allValues.push_back(value);
    }
  }
  return (stol(allValues[21])/sysconf(_SC_CLK_TCK));
}
