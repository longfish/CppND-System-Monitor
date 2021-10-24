#include <iostream>
#include <vector>

#include "format.h"
#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  NCursesDisplay::Display(system);

  // test
  // std::cout << LinuxParser::UpTime() << "\n";
  // std::cout << Format::ElapsedTime(system.UpTime()) << "\n";
  // std::vector<long> cpu_stat = LinuxParser::CpuUtilization();
  // for (auto ss : cpu_stat) {
  //   std::cout << ss << "\n";
  // }
  // Process proc;
  // proc.Pid(115);
  // std::cout << proc.Ram() << "\n";
}