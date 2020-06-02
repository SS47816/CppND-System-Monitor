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

// TODO: Return the system's CPU
Processor& System::Cpu() {
  return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  kernel_ = LinuxParser::Kernel();
  return kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  memory_utilization_ = LinuxParser::MemoryUtilization()
  return memory_utilization_;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
  operating_system_ = LinuxParser::OperatingSystem();
  return operating_system_;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  running_processes_ = LinuxParser::RunningProcesses();
  return running_processes_;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  total_processes_ = LinuxParser::TotalProcesses();
  return total_processes_;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  up_time_ = LinuxParser::UpTime();
  return up_time_;
}