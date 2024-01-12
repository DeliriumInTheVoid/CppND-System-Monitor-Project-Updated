#include <string>

#include "format.h"

#include <string>
#include <string>
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long times) {
  const int hours = static_cast<int>(times / 3600);
  const int minutes = static_cast<int>((times - hours * 3600) / 60);
  const int seconds = static_cast<int>(times - hours * 3600 - minutes * 60);

  return (hours > 9 ? "" : "0") + std::to_string(hours) + ":" +
         (minutes > 9 ? "" : "0") + std::to_string(minutes) + ":" +
         (seconds > 9 ? "" : "0") + std::to_string(seconds);
}
