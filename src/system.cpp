#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::AggregatedCpu() { return cpu_; }

vector<Process>& System::Processes() {
  processes_.clear();

  auto pids = LinuxParser::Pids();

  for (const auto pid : pids) {
    auto process = Process(pid);
    processes_.push_back(process);
  }

  std::sort(processes_.rbegin(), processes_.rend());

  return processes_;
}

vector<Processor>& System::Cpus() {
  cpus_.clear();

  const auto cpu_ids = LinuxParser::TotalCpus();

  for (int cpu_id = 0; cpu_id < cpu_ids; ++cpu_id) {
    auto cpu = Processor(LinuxParser::kStatCpuKey + std::to_string(cpu_id));
    cpus_.push_back(cpu);
  }

  return cpus_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
