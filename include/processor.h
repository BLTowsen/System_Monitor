#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "linux_parser.h"
#include <string>
#include <vector> 
class Processor {
 public:
  Processor(){
  std::vector<std::string> util = LinuxParser::CpuUtilization();
  int user = std::stoi(util[0])+std::stoi(util[8]);
  int nice = std::stoi(util[1])+std::stoi(util[9]);
  int idle = std::stoi(util[3])+std::stoi(util[4]);
  int system = std::stoi(util[2])+std::stoi(util[5])+std::stoi(util[6]);
  int virtal = std::stoi(util[8])+std::stoi(util[9]);
  float totaltime = user + nice + system + idle + std::stoi(util[7]) + virtal;
  CpuUtilization_ = (totaltime - idle)/totaltime;
  };
  float Utilization();  // TODO: See src/processor.cpp
  // TODO: Declare any necessary private members
 private:
 float CpuUtilization_;
};

#endif