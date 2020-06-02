#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  const auto hours = seconds / 3600;
  const auto minutes = (seconds % 3600) / 60;
  const auto remainder = seconds % 60;
  const string time = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(remainder);
  return time;
}