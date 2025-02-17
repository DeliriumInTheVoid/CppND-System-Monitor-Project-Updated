#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

const std::string kMeminfoMemTotalKey{"MemTotal:"};
const std::string kMeminfoMemFreeKey{"MemFree:"};

const std::string kStatCpuKey{"cpu"};
const std::string kStatProcessesKey{"processes"};
const std::string kStatRunningProcessesKey{"procs_running"};

const std::string kStatusVmSizeKey{"VmSize:"};
const std::string kStatusUidKey{"Uid:"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalCpus();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization(const std::string& cpu_id);
long Jiffies(const std::string& cpu_id);
long ActiveJiffies(const std::string& cpu_id);
long ActiveJiffies(int pid);
long IdleJiffies(const std::string& cpu_id);

// Processes
enum ProcessStates {
  kUtime = 13,
  kStime_,
  kCutime_,
  kCstime_,
  kStarttime_ = 21,
};
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif