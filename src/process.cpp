#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
using namespace std;

// Return this process's ID
int Process::Pid() { return id_; }

// Return this process's CPU utilization
// Refer to the answer of Vilhelm Gray:
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
float Process::CpuUtilization() const {
  long active_time = LinuxParser::ActiveJiffies(id_) / sysconf(_SC_CLK_TCK);
  long elapsed_time = LinuxParser::UpTime() - UpTime();
  return static_cast<float>(active_time) / elapsed_time;
}

// Return the command that generated this process (command length <= 50)
string Process::Command() {
  string command = LinuxParser::Command(id_);
  string commandCut = command.substr(0, 50);

  if (command.length() > 50) {
    commandCut = commandCut + "...";
  }
  return commandCut;
}

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(id_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(id_); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(id_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < CpuUtilization();
}