#include "process.h"

#include <unistd.h>

#include <cctype>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::cin;
using std::locale;
using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return id_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// helper struct for seperate input stream using colon
struct colon_is_space : std::ctype<char> {
  colon_is_space() : std::ctype<char>(get_table()) {}
  static mask const* get_table() {
    static mask rc[table_size];
    rc[':'] = std::ctype_base::space;
    rc['\n'] = std::ctype_base::space;
    return &rc[0];
  }
};

// TODO: Return the user (name) that generated this process
string Process::User() {
  // find the UID of the process
  string key, line, uid, value;
  std::ifstream filestream1(LinuxParser::kProcDirectory + to_string(id_) +
                            LinuxParser::kStatusFilename);
  if (filestream1.is_open()) {
    while (std::getline(filestream1, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }

  // find the corresponding username
  // refer to Robᵩ answer:
  // https://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
  string uname, x;
  std::ifstream filestream2(LinuxParser::kPasswordPath);
  if (filestream2.is_open()) {
    while (std::getline(filestream2, line)) {
      std::istringstream linestream(line);
      linestream.imbue(locale(linestream.getloc(), new colon_is_space));

      while (linestream >> key >> x >> value) {
        if (value == uid) {
          uname = key;
        }
      }
    }
  }
  return uname;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}