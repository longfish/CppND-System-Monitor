#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using namespace LinuxParser;
using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// helper function for sorting processes
bool Compare(Process& a, Process& b) { return a < b; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  vector<int> pids = LinuxParser::Pids();
  for (int id : pids) {
    Process proc;
    proc.Pid(id);
    processes_.emplace_back(proc);
  }

  // sort the processes based on cpu utilization (larger first)
  std::sort(processes_.begin(), processes_.end(), Compare);
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }