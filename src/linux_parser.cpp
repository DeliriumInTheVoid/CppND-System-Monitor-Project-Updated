#include <string>
#include <vector>
#include <filesystem>

#include <dirent.h>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;

  for (const auto& entry :
       std::filesystem::directory_iterator(kProcDirectory)) {
    if (entry.is_directory()) {
      std::string entryName = entry.path().filename();
      if (entryName.find_first_not_of("0123456789") == std::string::npos) {
        int pid = std::stoi(entryName);
        pids.push_back(pid);
      }
    }
  }

  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  long long total_memory{};
  long long free_memory{};

  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  while (stream.is_open()) {
    std::getline(stream, line);

    std::istringstream linestream(line);

    string line_info_id{};
    linestream >> line_info_id;

    if (line_info_id == kMeminfoMemTotalKey) {
      string total_memory_str{};
      linestream >> total_memory_str;
      total_memory = stoll(total_memory_str);
    } else if (line_info_id == kMeminfoMemFreeKey) {
      string free_memory_str{};
      linestream >> free_memory_str;
      free_memory = stoll(free_memory_str);
    }

    if (total_memory != 0 && free_memory != 0) {
      return static_cast<float>(total_memory - free_memory) / static_cast<float>(total_memory);
    }
  }

  return 0.0;
}

long LinuxParser::UpTime() {
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    string uptime_seconds{};
    linestream >> uptime_seconds;
    return stol(uptime_seconds);
  }

  return 0;
}

long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

long LinuxParser::ActiveJiffies(int pid) {

  int counter{17};

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  vector<string> utilization_values{};
  utilization_values.reserve(counter);

  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);

    string line_info_id{};
    linestream >> line_info_id;

    string value_str{};

    while (linestream >> value_str && counter-- > 0) {
      utilization_values.push_back(value_str);
    }

    const long utime = std::stoll(utilization_values[kUtime]);
    const long stime = std::stoll(utilization_values[kStime_]);
    const long cutime = std::stoll(utilization_values[kCutime_]);
    const long cstime = std::stoll(utilization_values[kCutime_]);

    const long total_time = utime + stime + cutime + cstime;

    return total_time;
  }

  return 0;
}

long LinuxParser::ActiveJiffies() {
  const auto cpu_utilization = CpuUtilization();

  return std::stoll(cpu_utilization[kUser_]) +
         std::stoll(cpu_utilization[kNice_]) + 
         std::stoll(cpu_utilization[kSystem_]) + 
         std::stoll(cpu_utilization[kIRQ_]) + 
         std::stoll(cpu_utilization[kSoftIRQ_]) + 
         std::stoll(cpu_utilization[kSteal_]);
}

long LinuxParser::IdleJiffies() {
  const auto cpu_utilization = CpuUtilization();

  return std::stoll(cpu_utilization[kIdle_]) +
         std::stoll(cpu_utilization[kIOwait_]);
}

vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);

  vector<string> utilization_values{};
  utilization_values.reserve(10);

  while (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);

    string line_info_id{};
    linestream >> line_info_id;

    if (line_info_id == kStatCpuKey) {
      string value_str{};

      while (linestream >> value_str) {
        utilization_values.push_back(value_str);
      }
    }

    return utilization_values;
  }

  return {};
}

int LinuxParser::TotalProcesses() {
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    string line_info_id{};
    linestream >> line_info_id;

    if (line_info_id == kStatProcessesKey) {
      string total_processes{};
      linestream >> total_processes;
      return stoi(total_processes);
    }
  }

  return 0;
}

int LinuxParser::RunningProcesses() {

  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    string line_info_id{};
    linestream >> line_info_id;

    if (line_info_id == kStatRunningProcessesKey) {
      string running_processes{};
      linestream >> running_processes;
      return stoi(running_processes);
    }
  }

  return 0;
}

string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    if (line.size() > 30) {
      return line.substr(0, 29) + "...";
    }
    return line;
  }

  return string{""};
}

string LinuxParser::Ram(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    string line_info_id{};
    linestream >> line_info_id;

    if (line_info_id == kStatusVmSizeKey) {
      string ram_size{};
      linestream >> ram_size;
      return to_string(stoi(ram_size) / 1024);
    }
  }

  return string{"0"};
}

string LinuxParser::Uid(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    string line_info_id{};
    linestream >> line_info_id;

    if (line_info_id == kStatusUidKey) {
      string user_id{};
      linestream >> user_id;
      return user_id;
    }
  }
  return string("");
}

string LinuxParser::User(int pid) {
  const auto uid_search_str = ":x:" + Uid(pid);
  string line;
  std::ifstream stream(kPasswordPath);
  while (stream.is_open()) {
    std::getline(stream, line);
    const size_t id_index = line.find(uid_search_str);
    if (id_index != string::npos) {
      return line.substr(0, id_index);
    }
  }

  return string{};
}

long LinuxParser::UpTime(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    string value{};
    linestream >> value;
    for (int i = 0; i < kStarttime_; ++i) {
      linestream >> value;
    }
    return UpTime() - stol(value) / sysconf(_SC_CLK_TCK);
  }
  return 0;
}
