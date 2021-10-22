#include <iostream>
#include <vector>

#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  NCursesDisplay::Display(system);

  // test
  // std::cout << LinuxParser::Jiffies() << "\n";
  // std::vector<long> cpu_stat = LinuxParser::CpuUtilization();
  // for (auto ss : cpu_stat) {
  //   std::cout << ss << "\n";
  // }
}