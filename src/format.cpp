#include <string>
#include <chrono>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

// Original Implementation
string Format::ElapsedTime(long seconds) {
  const auto hours = int(seconds / 3600);
  auto remainder = seconds - hours * 3600;
  const auto minutes = remainder / 60;
  remainder = remainder - minutes * 60;

  string hh = hours >= 10? std::to_string(hours) : "0" + std::to_string(hours);
  string mm = minutes >= 10? std::to_string(minutes) : "0" + std::to_string(minutes);
  string ss = remainder >= 10? std::to_string(remainder) : "0" + std::to_string(remainder);
  
  return hh + ":" + mm + ":" + ss;
}

// Alternative Solution #1
// std::string Pad(string s, char c) {
//   s.insert(s.begin(), 2 - s.size(), c);
//   return s;
// }

// string Format::ElapsedTime(long time) {
//   int hours = time / (60 * 60);
//   int minutes = (time / 60) % 60;
//   long seconds = time % 60;
//   return std::string(Pad(std::to_string(hours), '0') + ":" +
//                      Pad(std::to_string(minutes), '0') + ":" +
//                      Pad(std::to_string(seconds), '0'));
// }

// Alternative Solution #2
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