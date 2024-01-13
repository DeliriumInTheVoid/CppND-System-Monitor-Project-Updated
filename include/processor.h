#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>


struct CpuJiffies {
  long active;
  long total;
};

class Processor {
public:
  explicit Processor(std::string cpu_id) : cpu_id_{std::move(cpu_id)}
  {}

 public:
  float Utilization();
  const std::string& CpuId() const { return cpu_id_; }

 private:
  std::string cpu_id_{};
  CpuJiffies last_cpu_jiffies_{};
};

#endif