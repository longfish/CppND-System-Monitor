#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hour = seconds / 3600;
  long sec = seconds % 3600;

  long min = sec / 60;
  sec = sec % 60;

  int nlimit = 2;  // limit length of the time number

  string h = std::to_string(hour);
  string m = std::to_string(min);
  string s = std::to_string(sec);
  h = string(nlimit - h.length(), '0') + h;
  m = string(nlimit - m.length(), '0') + m;
  s = string(nlimit - s.length(), '0') + s;

  return h + ':' + m + ':' + s;
}