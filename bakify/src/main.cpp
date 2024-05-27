#include <argparse/argparse.hpp>

constexpr const char *version{"0.0.1"};

int main(int argc, char **argv) {
  argparse::ArgumentParser argument_parser{"Bakify", version,
                                           argparse::default_arguments::none};
  return 0;
}
