#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
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