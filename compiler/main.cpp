#include "cli11/CLI11.hpp"

#include "Compiler.h"
#include "ConsoleErrorReporter.h"

int main(int argc, char ** argv) {
  CLI::App app{"til interface language compiler"};

  std::string file;
  std::string out;
  bool pretty;
  app.add_option("-f,--file", file, "The full path to the file to compile")->required(true);
  app.add_option("-o,--out", out, "The full path for the output")->required(true);
  app.add_flag("-p,--pretty", pretty, "Pretty prints the json output");
  CLI11_PARSE(app, argc, argv);

  auto error_reporter = std::make_shared<til::ConsoleErrorReporter>();
  Compiler compiler(file, out, error_reporter, pretty);
  if (compiler.Compile()) {
    return 0;
  }
  return 1;
}
