/*
 *  main.cpp - CLI setup and pass along to thread pool
 *  Copyright (C) 2024  Sebastian Pineda (spineda.wpi.alum@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include <argparse/argparse.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "backup_factory.hpp"

constexpr const char *version{"1.0.0"};

std::vector<std::string> CollectFilenames(const std::string &&file_name) {
  if (!std::filesystem::exists(file_name)) {
    std::cerr << "WARNING: " << file_name << " for -f could not be found!"
              << std::endl;
    return std::vector<std::string>{};
  } else {
    std::ifstream list_file{file_name};
    std::vector<std::string> files{};
    std::string line{};

    while (std::getline(list_file, line)) {
      files.emplace_back(line);
    }

    return files;
  }
}

int main(int argc, char **argv) {
  argparse::ArgumentParser argument_parser{"Bakify", version,
                                           argparse::default_arguments::none};

  argument_parser.add_argument("-h", "--help")
      .flag()
      .help("Display this help message and exit");

  argument_parser.add_argument("-v", "--version")
      .flag()
      .help("Display version and copyright information");

  argument_parser.add_argument("-f", "--file")
      .help("Specify backing up files from a list on disk");

  argument_parser.add_argument("files")
      .nargs(argparse::nargs_pattern::any)
      .help("Files to create backups for");

  try {
    argument_parser.parse_args(argc, argv);
  } catch (const std::exception &error) {
    std::cerr << error.what() << std::endl;
    std::cerr << argument_parser;
    return -1;
  }

  if (argument_parser.get<bool>("-h")) {
    std::cout << argument_parser;
    return 0;
  } else if (argument_parser.get<bool>("-v")) {
    std::cout << "Bakify " << version << std::endl;
    std::cout
        << "Copyright (c) 2024 Sebastian Pineda (spineda.wpi.alum@gmail.com)"
        << std::endl;
    std::cout
        << "This program is free software; you may redistribute it under the "
           "terms of the\nGNU General Public License version 2 or (at your "
           "option) any later version. This\nprogram has absolutely no "
           "warranty."
        << std::endl
        << std::endl;
    return 0;
  }

  const std::optional<std::string> file_name{argument_parser.present("file")};
  if (file_name.has_value()) {
    StartBackingUp(argument_parser.get<std::vector<std::string>>("files"),
                   CollectFilenames(std::move(file_name.value())));
  } else {
    StartBackingUp(argument_parser.get<std::vector<std::string>>("files"),
                   std::vector<std::string>{});
  }

  return 0;
}
