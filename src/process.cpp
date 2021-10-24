#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return id_; }

// TODO: Return this process's CPU utilization
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
float Process::CpuUtilization() const {
  long active_time = LinuxParser::ActiveJiffies(id_) / sysconf(_SC_CLK_TCK);
  long elapsed_time = LinuxParser::UpTime() - UpTime();
  return (float)active_time / elapsed_time;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(id_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(id_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(id_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(id_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < CpuUtilization();
}