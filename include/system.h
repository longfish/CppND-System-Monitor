#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();  // TODO: See src/system.cpp; done in system
  std::vector<Process>&
  Processes();                // TODO: See src/system.cpp; done in system
  float MemoryUtilization();  // TODO: See src/system.cpp; done in linux_parser
  long UpTime();              // TODO: See src/system.cpp; done in linux_parser
  int TotalProcesses();       // TODO: See src/system.cpp; done in linux_parser
  int RunningProcesses();     // TODO: See src/system.cpp; done in linux_parser
  std::string Kernel();       // TODO: See src/system.cpp; done in linux_parser
  std::string
  OperatingSystem();  // TODO: See src/system.cpp; done in linux_parser

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif