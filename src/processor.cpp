#include <string.h>

#include "processor.h"
#include "linux_parser.h"

using std::string;

float Processor::Utilization() {
  const auto active = LinuxParser::ActiveJiffies();
  const auto total = LinuxParser::Jiffies();

  const auto active_diff = active - last_cpu_jiffies_.active;
  const auto total_diff = total - last_cpu_jiffies_.total;

  last_cpu_jiffies_.active = active;
  last_cpu_jiffies_.total = total;

  const auto utilization =
      static_cast<float>(active_diff) / static_cast<float>(total_diff);

  return utilization;
}