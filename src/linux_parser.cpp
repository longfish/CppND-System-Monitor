#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <locale>
#include <string>
#include <vector>
using namespace std;

// Find the value associated with a key in the file 'filename' in /proc
// directory
template <typename T>
T findValueByKey(string const& keyFilter, string const& filename) {
  string line;
  string key;
  T value;

  ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

// Read the single line data of the file 'filename' in /proc directory
template <typename T>
T getValueOfFile(string const& filename) {
  string line;
  T value;

  ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

// Read the operating system data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read linux kernel data from the filesystem
string LinuxParser::Kernel() {
  string os;
  string kernel;
  string version;
  string line;

  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (all_of(filename.begin(), filename.end(), ::isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization, from code reviewer
float LinuxParser::MemoryUtilization() {
  float Total = findValueByKey<float>(filterMemTotalString,
                                      kMeminfoFilename);  // "/proc/memInfo"
  float Free = findValueByKey<float>(filterMemFreeString, kMeminfoFilename);
  return (Total - Free) / Total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long upTime{0};
  long idleTime{0};
  string line;
  ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return upTime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<long> jiffies = CpuUtilization();
  long total = jiffies[CPUStates::kIdle_] + jiffies[CPUStates::kIOwait_] +
               jiffies[CPUStates::kUser_] + jiffies[CPUStates::kNice_] +
               jiffies[CPUStates::kSystem_] + jiffies[CPUStates::kIRQ_] +
               jiffies[CPUStates::kSoftIRQ_] + jiffies[CPUStates::kSteal_];
  return total;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> statValues = CpuStat(pid);
  long active = stol(statValues[13]) + stol(statValues[14]) +
                stol(statValues[15]) + stol(statValues[16]);

  return active;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> jiffies = CpuUtilization();
  long active = jiffies[CPUStates::kUser_] + jiffies[CPUStates::kNice_] +
                jiffies[CPUStates::kSystem_] + jiffies[CPUStates::kIRQ_] +
                jiffies[CPUStates::kSoftIRQ_] + jiffies[CPUStates::kSteal_];
  return active;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long> jiffies = CpuUtilization();
  long idle = jiffies[CPUStates::kIdle_] + jiffies[CPUStates::kIOwait_];
  return idle;
}

// Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  vector<long> jiffies{};
  string line;
  string key;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key;
      if (key == filterCpu) {
        long jif;
        while (linestream >> jif) {
          jiffies.emplace_back(jif);
        }
      }
    }
  }
  return jiffies;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key = filterProcesses;
  int nProc = findValueByKey<int>(key, kStatFilename);

  return nProc;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int nProcRunning = findValueByKey<int>(filterRunningProcesses, kStatFilename);

  return nProcRunning;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  return string(getValueOfFile<string>(to_string(pid) + kCmdlineFilename));
}

// Read and return the memory used by a process
// taken by the reviewer's advice, use VmRSS instead of VmSize to get an
// exact physical memory
string LinuxParser::Ram(int pid) {
  long ram =
      findValueByKey<long>(filterProcMem, to_string(pid) + kStatusFilename);

  return to_string(ram / 1000);  // convert from kb to mb in decimal
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return findValueByKey<string>(filterUID, to_string(pid) + kStatusFilename);
}

// Helper struct for seperate input stream using colon
// Refer to answer of Robᵩ:
// https://stackoverflow.com/questions/7302996/changing-the-delimiter-for-cin-c
struct colon_is_space : ctype<char> {
  colon_is_space() : ctype<char>(get_table()) {}
  static mask const* get_table() {
    static mask rc[table_size];
    rc[':'] = ctype_base::space;
    rc['\n'] = ctype_base::space;
    return &rc[0];
  }
};

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string key, line, value, uname, x;
  string uid = Uid(pid);
  ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream.imbue(
          locale(linestream.getloc(),
                 new colon_is_space));  // seperate input stream using colon

      while (linestream >> key >> x >> value) {
        if (value == uid) {
          uname = key;
        }
      }
    }
  }
  return uname;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  vector<string> stat_values = CpuStat(pid);
  long int age = stol(stat_values[21]) / sysconf(_SC_CLK_TCK);

  return age;
}

// Helper function: get the CPU stat values read from /proc/[pid]/stat
vector<string> LinuxParser::CpuStat(int pid) {
  vector<string> stat_values{};
  string line, value;
  ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                      LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    while (getline(linestream, value, ' ')) {
      stat_values.emplace_back(value);
    }
  }
  return stat_values;
}