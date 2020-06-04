#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Template functions for parsing data from file
template<typename T> 
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template<typename T> 
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

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
  string os, place_holder, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> place_holder >> kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float mem_total;
  float mem_free;
  const auto mem_total_string = findValueByKey<string>("MemTotal:", kProcDirectory + kMeminfoFilename);
  const auto mem_free_string = findValueByKey<string>("MemFree:", kProcDirectory + kMeminfoFilename);
  try {
    mem_total = std::stof(mem_total_string);
    mem_free = std::stof(mem_free_string);
  }
  // catch invalid_argument exception. 
  catch (const std::invalid_argument) { 
    std::cerr << "Invalid argument" << "\n"; 
    return 0.0f;
  } 

  const auto mem_usage = (mem_total - mem_free) / mem_total;
  return mem_usage > 0.0f ? mem_usage : 0.0f;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  const auto up_time = getValueOfFile<string>(kProcDirectory + kUptimeFilename);
  try {
    return std::stol(up_time);
  }
  // catch invalid_argument exception. 
  catch (const std::invalid_argument) { 
    std::cerr << "Invalid argument" << "\n"; 
  }
  return 0L;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  const auto active_all_time = LinuxParser::ActiveJiffies();
  const auto idle_all_time = LinuxParser::IdleJiffies();

  return active_all_time + idle_all_time;
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long total_time = 0;
  const int kStart = 13;
  long utime;
  long stime;
  // long cutime;
  // long cstime;
  string line;
  vector<string> values;
  const string kPidFilename = "/" + std::to_string(pid);
  std::ifstream filestream(kProcDirectory + kPidFilename + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i <= kStart + CPUStates::kGuestNice_; i++) {
        string value;
        linestream >> value;
        values.push_back(value);
      }
      try {
        utime = std::stoi(values[kStart]);
        stime = std::stol(values[kStart + 1]);
        // cutime = std::stol(values[kStart + 2]);
        // cstime = std::stol(values[kStart + 3]);
        total_time = utime + stime;
      }
      // catch invalid_argument exception. 
      catch (const std::invalid_argument) { 
        std::cerr << "Invalid argument" << "\n"; 
      }

      return total_time;
    }
  }
  
  return total_time;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active_all_time = 0;
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
          for (int i = 0; i <= CPUStates::kGuestNice_; i++) {
            string value;
            stringstream >> value;
            values.push_back(value);
          }
          try {
            user = std::stoi(values[CPUStates::kUser_]);
            nice = std::stol(values[CPUStates::kNice_]);
            system = std::stol(values[CPUStates::kSystem_]);
            irq = std::stol(values[CPUStates::kIRQ_]);
            softirq = std::stol(values[CPUStates::kSoftIRQ_]);
            steal = std::stol(values[CPUStates::kSteal_]);
            active_all_time = user + nice + system + irq + softirq + steal;
          }
          // catch invalid_argument exception. 
          catch (const std::invalid_argument) { 
            std::cerr << "Invalid argument" << "\n"; 
          }
          
          return active_all_time;
        }
      }
    }
  }

  return active_all_time;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idle_all_time = 0;
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
          for (int i = 0; i <= CPUStates::kGuestNice_; i++) {
            string value;
            stringstream >> value;
            values.push_back(value);
          }
          try {
            idle = std::stol(values[CPUStates::kIdle_]);
            iowait = std::stol(values[CPUStates::kIOwait_]);
            idle_all_time = idle + iowait;
          }
          // catch invalid_argument exception. 
          catch (const std::invalid_argument) { 
            std::cerr << "Invalid argument" << "\n"; 
          }

          return idle_all_time;
        }
      }
    }
  }

  return idle_all_time;
}

// NOT USED: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { 
//   return {};
// }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  const auto total_processes = findValueByKey<string>("processes", kProcDirectory + kStatFilename);
  try {
    return std::stoi(total_processes);
  }
  // catch invalid_argument exception. 
  catch (const std::invalid_argument) { 
    std::cerr << "Invalid argument" << "\n"; 
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  const auto procs_running = findValueByKey<string>("procs_running", kProcDirectory + kStatFilename);
  try {
    return std::stoi(procs_running);
  }
  // catch invalid_argument exception. 
  catch (const std::invalid_argument) { 
    std::cerr << "Invalid argument" << "\n"; 
  }
  return 0;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd = string();
  string line;
  vector<string> values;
  const string kPidFilename = "/" + std::to_string(pid);
  std::ifstream filestream(kProcDirectory + kPidFilename + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      cmd = line;
      return cmd;
    }
  }
  
  return cmd;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  const string kPidFilename = "/" + std::to_string(pid);
  // Using `VmData` instead of `VmSize` to measure virtual memory utilization
  const auto mem_string = findValueByKey<string>("VmData:", kProcDirectory + kPidFilename + kStatusFilename);
  try {
    return std::stol(mem_string) >= 1000L? std::to_string(std::stol(mem_string)/1000) : "0." + mem_string;
  }
  // catch invalid_argument exception. 
  catch (const std::invalid_argument) { 
    std::cerr << "Invalid argument" << "\n"; 
  } 
  
  return "N.A.";
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  const string kPidFilename = "/" + std::to_string(pid);
  return findValueByKey<string>("Uid:", kProcDirectory + kPidFilename + kStatusFilename);
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user = string();
  const string uid = LinuxParser::Uid(pid);
  string line;
  string key;
  string x;
  string value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (value == uid) {
          user = key;
          return user;
        }
      }
    }
  }

  return user;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long lifespan = 0;
  const int kStart = 13;
  long starttime;
  string line;
  vector<string> values;
  const string kPidFilename = "/" + std::to_string(pid);
  std::ifstream filestream(kProcDirectory + kPidFilename + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i <= kStart + CPUStates::kGuestNice_; i++) {
        string value;
        linestream >> value;
        values.push_back(value);
      }
      try {
        starttime = std::stol(values[kStart + 8]);
        const auto uptime = LinuxParser::UpTime();
        const auto Hertz = sysconf(_SC_CLK_TCK)*1.0f;
        lifespan = uptime - (starttime / Hertz);
      }
      // catch invalid_argument exception. 
      catch (const std::invalid_argument) { 
        std::cerr << "Invalid argument" << "\n"; 
      }

      return lifespan;
    }
  }
  
  return lifespan;
}