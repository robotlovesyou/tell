#include "cli11/CLI11.hpp"

#include "Compiler.h"
#include "ConsoleErrorReporter.h"

int main(int argc, char ** argv) {
  CLI::App app{"til interface language compiler"};

  std::string file; // The path to the source file
  std::string out; // The path to the AST output file
  bool pretty = false; // Whether to pretty print the AST JSON

  // Set up the app options and parse
  app.add_option("-f,--file", file, "The full path to the file to compile")->required(true);
  app.add_option("-o,--out", out, "The full path for the output")->required(true);
  app.add_flag("-p,--pretty", pretty, "Pretty prints the json output");
  CLI11_PARSE(app, argc, argv);

  // Prepare and execute the compiler
  auto error_reporter = std::make_shared<til::ConsoleErrorReporter>();
  Compiler compiler(file, out, error_reporter, pretty);
  if (compiler.Compile()) {
    return 0;
  }
  return 1;
}
