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
  return this->cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  this->pid_ = LinuxParser::Pids();
  std::vector<Process> processes;
  for (auto pid : pid_) {
    Process process(pid);
    processes.push_back(process);
  }
  this->processes_ = processes;

  return this->processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() {
  this->kernel_ = LinuxParser::Kernel();
  return this->kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  this->memory_utilization_ = LinuxParser::MemoryUtilization();
  return this->memory_utilization_;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() {
  this->operating_system_ = LinuxParser::OperatingSystem();
  return this->operating_system_;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  this->running_processes_ = LinuxParser::RunningProcesses();
  return this->running_processes_;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  this->total_processes_ = LinuxParser::TotalProcesses();
  return this->total_processes_;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
  this->up_time_ = LinuxParser::UpTime();
  return this->up_time_;
}