#include "format.h"

#include <string>

using std::string;

// Transform the seconds into hours::minutes::seconds form
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long hour = seconds / 3600;
  long sec = seconds % 3600;
  long min = sec / 60;
  sec = sec % 60;

  int nlimit = 2;  // limit length of each entity

  string h = std::to_string(hour);
  string m = std::to_string(min);
  string s = std::to_string(sec);
  h = string(nlimit - h.length(), '0') + h;
  m = string(nlimit - m.length(), '0') + m;
  s = string(nlimit - s.length(), '0') + s;

  return h + ':' + m + ':' + s;
}

// Another implementation given by reviewer
// string Format::ElapsedTime(long s) {
//   std::chrono::seconds seconds{s};

//   // return std::chrono::format("%T", seconds); // in C++20 :-)

//   std::chrono::hours hours =
//       std::chrono::duration_cast<std::chrono::hours>(seconds);

//   seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);

//   std::chrono::minutes minutes =
//       std::chrono::duration_cast<std::chrono::minutes>(seconds);

//   seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

//   std::stringstream ss{};

//   ss << std::setw(2) << std::setfill('0') << hours.count()     // HH
//      << std::setw(1) << ":"                                    // :
//      << std::setw(2) << std::setfill('0') << minutes.count()   // MM
//      << std::setw(1) << ":"                                    // :
//      << std::setw(2) << std::setfill('0') << seconds.count();  // SS

//   return ss.str();
// }