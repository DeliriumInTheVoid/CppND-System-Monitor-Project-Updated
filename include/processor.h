#ifndef PROCESSOR_H
#define PROCESSOR_H

struct CpuJiffies {
  long active;
  long total;
};

class Processor {
 public:
  float Utilization();

 private:
  CpuJiffies last_cpu_jiffies_{};
};

#endif