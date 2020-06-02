#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  system_all_time_ = LinuxParser::Jiffies();
  active_all_time_ = LinuxParser::ActiveJiffies();

  utilization_ = active_all_time_ * 1.0f / system_all_time_ * 100.0f;
  
  return utilization_;
}