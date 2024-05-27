#include <argparse/argparse.hpp>
#include <exception>
#include <iostream>
#include <ostream>

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

  return 0;
}
