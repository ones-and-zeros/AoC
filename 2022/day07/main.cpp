#include <cassert>
#include <cstdint>
#include <limits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "timer.h"

struct File {
  std::string name;
  std::size_t size;
};

struct Directory {
  explicit Directory(std::string name_, Directory* parent_)
  : name{name_}, parent{parent_}, sub_dirs{}, files{}
  {}

  std::string name;
  Directory* parent;
  std::vector<Directory> sub_dirs;
  std::vector<File> files;
};

std::size_t DirectorySize(const Directory& dir) {
  std::size_t size = 0;

  for (const auto& sub : dir.sub_dirs) {
    size += DirectorySize(sub);
  }

  for (const auto& file : dir.files) {
    size += file.size;
  }

  return size;
}

std::size_t CalcPart1(const Directory& dir) {
  std::size_t total = 0;

  for (const auto& sub : dir.sub_dirs) {
    const auto size = DirectorySize(sub);
    if (size <= 100000) {
      total += DirectorySize(sub);
    }
    total += CalcPart1(sub);
  }

  return total;
}

std::size_t part_2_target_to_free_ = 0;
std::size_t part_2_temp_ = 0;

void CheckPart2(const Directory& dir) {
  const std::size_t size = DirectorySize(dir);
  if(size >= part_2_target_to_free_ && size < part_2_temp_) {
    part_2_temp_ = size;
  }

  for (const auto& sub : dir.sub_dirs) {
    const std::size_t size = DirectorySize(sub);
    if(size >= part_2_target_to_free_ && size < part_2_temp_) {
      part_2_temp_ = size;
    }
    CheckPart2(sub);
  }
}

std::size_t CalcPart2(const Directory& dir) {
  constexpr std::size_t total_space{70000000};
  constexpr std::size_t required_free{30000000};

  const std::size_t currently_free = total_space - DirectorySize(dir);
  part_2_target_to_free_ = required_free - currently_free;
  part_2_temp_ = std::numeric_limits<std::size_t>::max();

  std::cout << "          used: " <<  DirectorySize(dir) << "\n";
  std::cout << "currently free: " <<  currently_free << "\n";
  std::cout << "  need to free: " <<  part_2_target_to_free_ << "\n";

  CheckPart2(dir);

  return part_2_temp_;
}

Directory* ParseLine(Directory* current_dir, const std::string& line) {
  assert(current_dir != nullptr);

  std::istringstream iss{line};
  std::string identifier_s;

  iss >> identifier_s;

  if(identifier_s == "$") {
    // command
    std::string command;
    iss >> command;

    if(command == "ls") {
      // no action needed
      return current_dir;
    }

    if(command == "cd") {
      std::string new_location;
      iss >> new_location;

      if(new_location == "/") {
        // root, but do nothing as only used for first command
        return current_dir;
      }

      if(new_location == "..") {
        return current_dir->parent;
      }

      for (std::size_t idx = 0; idx < current_dir->sub_dirs.size(); idx++) {
        if(current_dir->sub_dirs[idx].name == new_location) {
          return &(current_dir->sub_dirs[idx]);
        }
      }
      throw std::logic_error("invalid filename");    
    }
    throw std::logic_error("invalid command");    
  }

  if(identifier_s == "dir") {
    // directory
    std::string name;
    iss >> name;
    current_dir->sub_dirs.push_back(Directory{name, current_dir});
    return current_dir;
  }

  if(std::isdigit(identifier_s[0])) {
    // file
    std::string name;
    iss >> name;
    current_dir->files.push_back(File{name, std::stoul(identifier_s)});
    return current_dir;
  }

  throw std::logic_error("unexpected parsing");
}

void PrintTree(Directory* dir, const std::size_t depth) {
  assert(dir != nullptr);

  std::string lead(depth, ' ');
  lead += "- ";
  std::cout << lead << dir->name << " (dir)\n";

  for (std::size_t idx = 0; idx < dir->sub_dirs.size(); idx++) {
     PrintTree(&(dir->sub_dirs[idx]), depth + 1);
  }

  for (std::size_t idx = 0; idx < dir->files.size(); idx++) {
    std::cout << " " << lead << dir->files[idx].name
              << " (file, size=" << dir->files[idx].size << ")\n";
  }
}

int main()
{
  Timer t_main("main");

  Directory file_system{"root", nullptr};

  std::ifstream infile{"input.txt"};
  if (infile.is_open()) {
    Timer t_p("parse input");

    Directory* current_dir = &file_system;

    std::string line;
    while (getline(infile, line)) {
      current_dir = ParseLine(current_dir, line);
    }

    infile.close();
  }
  else
    throw std::logic_error("unable to open input file");
  std::cout << "\n";

  // // input parse check
  // PrintTree(&file_system, 0);
  // std::cout << "\n";

  std::size_t result_p1 = 0;
  std::size_t result_p2 = 0;

  {
    Timer t_main("calc");

    result_p1 = CalcPart1(file_system);
    result_p2 = CalcPart2(file_system);
  }

  std::cout << "\n";
  std::cout << "part 1) : " << result_p1 << "\n";
  std::cout << "part 2) : " << result_p2 << "\n";
  std::cout << "\n";
}
