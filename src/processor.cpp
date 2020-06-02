#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  this->system_all_time_ = LinuxParser::Jiffies();
  this->active_all_time_ = LinuxParser::ActiveJiffies();

  this->utilization_ = this->active_all_time_ * 1.0f / this->system_all_time_ * 100.0f;
  
  return this->utilization_;
}