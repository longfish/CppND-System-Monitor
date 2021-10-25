#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();
  void Pid(int id) { id_ = id; }
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime() const;
  bool operator<(Process const& a) const;

 private:
  int id_{};
};

#endif