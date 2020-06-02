#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float mem_total;
  float mem_free;
  // float mem_available;
  // float buffers;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> key >> value) {
        if (key == "MemTotal") mem_total = std::stof(value);
        else if (key == "MemFree") {
          mem_free = std::stof(value);
          const float mem_used = mem_total - mem_free;
          return mem_used > 0.0? mem_used : 0.0;
        }
        // else if (key == "MemAvailable") mem_available = std::stof(value);
        // else if (key == "Buffers") 
      }
    }
  }

  return mem_total - mem_free;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long up_time;
  string line;
  string value1;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    while (stream >> value1) {
      up_time = std::stol(value1);
      return up_time;
    }
  }

  return up_time;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  const auto active_all_time = LinuxParser::ActiveJiffies();
  const auto idle_all_time = LinuxParser::IdleJiffies();

  return active_all_time + idle_all_time;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active_all_time;
  long user;
  long nice;
  long system;
  long irq;
  long softirq;
  long steal;
  string line;
  string key;
  vector<string> values;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      if (stringstream >> key) {
        if (key == "cpu") {
          for (int i = 0; i < 10; i++) {
            stringstream >> values[i];
          }
          user = std::stoi(values[0]);
          nice = std::stol(values[1]);
          system = std::stol(values[2]);
          irq = std::stol(values[5]);
          softirq = std::stol(values[6]);
          steal = std::stol(values[7]);
          active_all_time = user + nice + system + irq + softirq + steal;
          return active_all_time;
        }
      }
    }
  }

  return active_all_time;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idle_all_time;
  long idle;
  long iowait;
  string line;
  string key;
  vector<string> values;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      if (stringstream >> key) {
        if (key == "cpu") {
          for (int i = 0; i < 10; i++) {
            stringstream >> values[i];
          }
          idle = std::stol(values[3]);
          iowait = std::stol(values[4]);
          idle_all_time = idle + iowait;
          return idle_all_time;
        }
      }
    }
  }

  return idle_all_time;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int total_processes;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          total_processes = std::stoi(value);
          return total_processes;
        }
      }
    }
  }

  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int procs_running;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          procs_running = std::stoi(value);
          return procs_running;
        }
      }
    }
  }

  return procs_running;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string uid;
  string line;
  string key;
  string value;
  const string kPidFilename = "/" + std::to_string(pid);
  std::ifstream filestream(kProcDirectory + kPidFilename + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          uid = value;
          return uid;
        }
      }
    }
  }
  
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }