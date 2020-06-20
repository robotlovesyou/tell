#include <fstream>
#include <iostream>

#include "cli11/CLI11.hpp"
#include "fmt/format.h"
#include "nlohmann/json.hpp"

#include "GoGenerator.h"
#include "SerializableAST.h"

using nlohmann::json;

/**
 * Open the file at path and read it into a serializable ast
 * @param file
 * @return
 */
til::SerializableAST ReadAST(const std::string& path);

/**
 * Generate Go Client and Server RPC code from the provided ast
 * @param ast
 * @param out_path
 */
void GenerateGo(til::SerializableAST &ast, const std::string &out_path);

int main(int argc, char**argv) {
  CLI::App app{"tell RPC code generator"};

  std::string file;
  std::string go_out;
  app.add_option("-f,--file", file, "The full path to the compiled til json")->required(true);
  app.add_option("--go_out", go_out, "The full path for the generated golang output");
  CLI11_PARSE(app, argc, argv);

  std::cout << "I will generate " << file << " to " << go_out << std::endl;

  til::SerializableAST ast;
  try {
    ast = ReadAST(file);
  } catch (std::exception &e) {
    std::cout << fmt::format("Encountered an error reading the input file: {}", e.what()) << std::endl;
    return 1;
  }

  if (go_out.empty()) {
    std::cout << "No output format requested." << std::endl;
    return 1;
  }

  if (!go_out.empty()) {
    try {
      GenerateGo(ast, go_out);
    } catch (std::exception &e) {
      std::cout << fmt::format("Encountered an error generating Go: {}", e.what());
    }
  }

}

til::SerializableAST ReadAST(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error(fmt::format("cannot open file at {}", path));
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  auto j = json::parse(buffer.str());
  return j.get<til::SerializableAST>();
}

void GenerateGo(til::SerializableAST &ast, const std::string &out_path) {
  std::ofstream file(out_path);
  if(!file.is_open()) {
    throw std::runtime_error(fmt::format("cannot open go output file at {}", out_path));
  }

  gen::GoGenerator generator;
  file << generator.Generate(ast);
  file.flush();
  file.close();
}

