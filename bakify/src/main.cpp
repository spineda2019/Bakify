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
#include <iostream>
#include <ostream>
#include <vector>

#include "backup_factory.hpp"

constexpr const char *version{"0.0.1"};

int main(int argc, char **argv) {
  argparse::ArgumentParser argument_parser{"Bakify", version,
                                           argparse::default_arguments::none};

  argument_parser.add_argument("-h", "--help")
      .flag()
      .help("Display this help message and exit");

  argument_parser.add_argument("-v", "--version")
      .flag()
      .help("Display version and copyright information");

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

  StartBackingUp(argument_parser.get<std::vector<std::string>>("files"));

  return 0;
}
