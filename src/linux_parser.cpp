#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <locale>
#include <string>
#include <vector>

using std::locale;
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
  string os, version, kernel;
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
  float temp{0.0}, mem_total{0.0}, mem_free{0.0};
  string key;
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> temp) {
        if (key == "MemTotal:") {
          mem_total = temp;
        }
        if (key == "MemFree:") {
          mem_free = temp;
        }
      }
    }
  }
  return (mem_total - mem_free) / mem_total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime{0}, idletime{0};
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<long> jiffies = CpuUtilization();
  long total = jiffies[CPUStates::kIdle_] + jiffies[CPUStates::kIOwait_] +
               jiffies[CPUStates::kUser_] + jiffies[CPUStates::kNice_] +
               jiffies[CPUStates::kSystem_] + jiffies[CPUStates::kIRQ_] +
               jiffies[CPUStates::kSoftIRQ_] + jiffies[CPUStates::kSteal_];
  return total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> stat_values = CpuStat(pid);
  long active = stol(stat_values[13]) + stol(stat_values[14]) +
                stol(stat_values[15]) + stol(stat_values[16]);

  return active;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> jiffies = CpuUtilization();
  long active = jiffies[CPUStates::kUser_] + jiffies[CPUStates::kNice_] +
                jiffies[CPUStates::kSystem_] + jiffies[CPUStates::kIRQ_] +
                jiffies[CPUStates::kSoftIRQ_] + jiffies[CPUStates::kSteal_];
  return active;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long> jiffies = CpuUtilization();
  long idle = jiffies[CPUStates::kIdle_] + jiffies[CPUStates::kIOwait_];
  return idle;
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  vector<long> jiffies{};
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        long jif;
        while (linestream >> jif) {
          jiffies.emplace_back(jif);
        }
      }
    }
  }
  return jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int n_proc{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> n_proc) {
        if (key == "processes") {
          return n_proc;
        }
      }
    }
  }
  return n_proc;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int n_proc_running{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> n_proc_running) {
        if (key == "procs_running") {
          return n_proc_running;
        }
      }
    }
  }
  return n_proc_running;
}

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

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value, ram;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          ram = to_string(stol(value) /
                          1000);  // convert from kb to mb in decimal
        }
      }
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string key, line, uid, value;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  // refer to Robᵩ answer:
  // https://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
  string key, line, value, uname, x;
  string uid = Uid(pid);
  std::ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
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

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  vector<string> stat_values = CpuStat(pid);
  long int age =
      stol(stat_values[21]) / sysconf(_SC_CLK_TCK);  // up time of the process

  return age;
}

// Helper function: get the CPU stat values read from /proc/[pid]/stat
vector<string> LinuxParser::CpuStat(int pid) {
  vector<string> stat_values{};
  string line, value;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (getline(linestream, value, ' ')) {
      stat_values.emplace_back(value);
    }
  }
  return stat_values;
}