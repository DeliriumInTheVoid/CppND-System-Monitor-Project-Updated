#include <string>
#include <vector>

#include "process.h"

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const {
  return static_cast<float>(LinuxParser::ActiveJiffies(pid_)) /
         static_cast<float>(LinuxParser::UpTime(pid_));
}

string Process::Command() {
  return LinuxParser::Command(pid_);
}

string Process::Ram() {
  return LinuxParser::Ram(pid_);
}

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { return CpuUtilization() < a.CpuUtilization(); }