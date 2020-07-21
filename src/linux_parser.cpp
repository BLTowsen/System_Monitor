#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include "linux_parser.h"

using std::stof;
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
  string os, kernel, temp;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> temp >> kernel;
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
   float total, free, memAvailable, buffer;
   string line;
   string description = "";
   std::ifstream stream(kProcDirectory + kMeminfoFilename); 
   if(stream.is_open()){
     	
       while(std::getline(stream,line)){
        std::istringstream stringstream(line);
        stringstream >> description;
        if(description == "MemTotal:"){
          stringstream >> total;
        }
        else if(description == "MemFree:"){
          stringstream >> free;
        }
        else if(description == "MemAvailable:"){
          stringstream >> memAvailable;
        }
        else if(description == "Buffers:"){
          stringstream >> buffer;
        }
       }
   }
  
   return (memAvailable-free)/memAvailable; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream stringstream(line);
    stringstream >> uptime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line, description;
  long jiffies_total = 0;
  long jiff;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
    std::istringstream stringstream(line);
    stringstream >> description;
    if(description == "cpu"){
      for(int i = 0; i<10; i++){
         stringstream >> jiff;
         jiffies_total = jiffies_total + jiff;
      }
      return jiffies_total;
    }
    }
  }
  return jiffies_total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long jiffies_total = 0;
  long jiffie;
  string line;
  string temp;
  std::ifstream stream(kProcDirectory+ std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream stringstream(line);
    for(int i = 0; i < 13; i++){
      stringstream >> temp;  
    }
    for(int i =14; i <=17; i++){
       stringstream >> jiffie;
      jiffies_total = jiffies_total + jiffie;
    }
    return jiffies_total;
  }
  return jiffies_total; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  long active_jiffies = 0;
  long jiffie;
  string temp;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
    std::istringstream stringstream(line);
    stringstream >> temp;
    if(temp == "cpu"){
      for(int i = 0; i < 3; i++){
        stringstream >> jiffie;
        active_jiffies = active_jiffies+jiffie;
      }
      stringstream >> temp >> temp;
      for(int i = 6; i <=9; i++){
        stringstream >> jiffie;
        active_jiffies = active_jiffies + jiffie;
      }
      return active_jiffies;
    }
    }
  }
  
  return active_jiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  return LinuxParser::Jiffies()-LinuxParser::ActiveJiffies(); 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuUtil;  
  string line, cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream,line)){
    std::istringstream linestream(line);
	  linestream >> cpu;
    if(cpu == "cpu"){
      while(linestream >> cpu){
        cpuUtil.push_back(cpu); 
      }
      return cpuUtil;
    }
    }
  }
  return cpuUtil; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_processes;
  string first = "";
  string line;
  std::ifstream stream(kProcDirectory+ kStatFilename);
  if(stream.is_open()){
      while(std::getline(stream,line)){
      std::istringstream stringstream(line);
      stringstream >>first >> total_processes;
      if(first == "processes"){
        return total_processes;
      }
      }
  }
  return total_processes; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string description{""};
  int totalProcesses;
  string line, temp;
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
      while(std::getline(stream, line)){
      std::istringstream stringstream(line);
      stringstream >> description >> temp;;
      if(description == "procs" && temp == "running"){
        stringstream >> totalProcesses; 
        return totalProcesses; 
      }
      }
  }
  return totalProcesses; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, command);
    return command;
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string description, line;
  float memory;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
    std::istringstream stringstream(line);
    stringstream >> description >> memory;
    if(description == "VmSize"){return std::to_string(memory/1000);}
    }                           
  }
  return std::to_string(memory/1000); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string userID, line, description;
  std::ifstream stream( kProcDirectory+std::to_string(pid)+ kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
    std::istringstream stringstream(line);
    stringstream>>description;
    if(description == "Uid:"){
      stringstream >> userID;
      return userID;
    }
    }
  }  
  return userID;}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string userID = LinuxParser::Uid(pid);
  string user;
  string temp;
  string ID;
  string line;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream stringstream(line);
    stringstream >> user >> temp >> ID;
      if(ID == userID){
        return user; 
      } 
    }        
  }  
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long uptime;
  string line;
  string temp;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream stringstream(line);
    for(int i=0; i<21; i++){
      stringstream >> temp;
    }
    stringstream >> uptime;

    return uptime;
  }
  return uptime; 
}