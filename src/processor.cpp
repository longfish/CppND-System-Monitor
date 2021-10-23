#include "processor.h"

#include <unistd.h>  // since this is a linux program

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float prev_active = (float)LinuxParser::ActiveJiffies(),
        prev_total = (float)LinuxParser::Jiffies();
  sleep(1);
  float cur_active = (float)LinuxParser::ActiveJiffies(),
        cur_total = (float)LinuxParser::Jiffies();
  return (cur_active - prev_active) / (cur_total - prev_total);
}
