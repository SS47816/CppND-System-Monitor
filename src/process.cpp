#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  this->pid_ = pid;
  this->user_ = LinuxParser::User(pid);
  this->command_ = LinuxParser::Command(pid);
  const auto total_time_= LinuxParser::ActiveJiffies(pid);
  const auto uptime = LinuxParser::UpTime(pid);
  const auto Hertz = sysconf(_SC_CLK_TCK)*1.0f;
  const auto seconds = uptime - (starttime / Hertz);
  const auto cpu_usage = 100.0f * ((total_time / Hertz) / seconds);
  this->ram_ = LinuxParser::Ram(pid);
  this->up_time_ = LinuxParser::UpTime(pid);
}

// TODO: Return this process's ID
int Process::Pid() {
  return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  this->up_time_ = LinuxParser::UpTime(this->pid_);
  return this->up_time_; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return this->cpu_utilization_ < a.cpu_utilization_? true : false;
}