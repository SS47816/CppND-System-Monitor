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

Process::Process(int pid) : pid_(pid) {}

// TODO: Return this process's ID
int Process::Pid() {
  return this->pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  const auto Hertz = sysconf(_SC_CLK_TCK)*1.0f;
  const auto total_time = LinuxParser::ActiveJiffies(this->pid_);
  const auto lifespan = LinuxParser::UpTime(this->pid_);
  this->cpu_utilization_ = 100.0f*((total_time / Hertz) / lifespan);
  return this->cpu_utilization_;
}

// TODO: Return the command that generated this process
string Process::Command() {
  this->command_ = LinuxParser::Command(this->pid_);
  return this->command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  this->ram_ = LinuxParser::Ram(this->pid_);
  return this->ram_;
}

// TODO: Return the user (name) that generated this process
string Process::User() {
  this->user_ = LinuxParser::User(this->pid_);
  return this->user_;
}

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