#include "cli11/CLI11.hpp"
#include <filesystem>
#include <iostream>

int main(int argc, char**argv) {
  CLI::App app{"tell RPC code generator"};

  std::string file;
  std::string go_out;
  app.add_option("-f,--file", file, "The full path to the compiled til json")->required(true);
  app.add_option("--go_out", go_out, "The full path for the generated golang output");
  CLI11_PARSE(app, argc, argv);

  std::cout << "I will generate " << file << " to " << go_out << std::endl;
}