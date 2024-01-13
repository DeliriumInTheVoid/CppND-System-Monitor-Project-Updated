#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& AggregatedCpu();
  std::vector<Process>& Processes();
  std::vector<Processor>& Cpus();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

 private:
  Processor cpu_{LinuxParser::kStatCpuKey};
  std::vector<Process> processes_ = {};
  std::vector<Processor> cpus_ = {};
};

#endif