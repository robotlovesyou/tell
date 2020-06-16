#include "cli11/CLI11.hpp"
#include <iostream>

int main(int argc, char ** argv) {
  CLI::App app{"til interface language compiler"};

  std::string file;
  std::string out;
  app.add_option("-f,--file", file, "The full path to the file to compile")->required(true);
  app.add_option("-o, --out", out, "The full path for the output")->required(true);
  CLI11_PARSE(app, argc, argv);

  std::cout << "I will compile: " << file << " to " << out << std::endl;
}
