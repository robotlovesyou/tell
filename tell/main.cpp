#include <fstream>
#include <iostream>

#include "cli11/CLI11.hpp"
#include "fmt/format.h"
#include "nlohmann/json.hpp"

#include "Generator.h"
#include "GoGenerator.h"
#include "HTMLGenerator.h"
#include "SerializableAST.h"

using nlohmann::json;

/**
 * Open the file at path and read it into a serializable ast
 * @param file
 * @return
 */
til::SerializableAST ReadAST(const std::string& path);

/**
 * Use the provided generator and ast to generate source to the provided path
 * @param ast
 * @param out_path
 * @param generator
 */
void Generate(const til::SerializableAST &ast, const std::string &out_path, gen::Generator &generator);

/**
 * Generate Go Client and Server RPC code from the provided ast
 * @param ast
 * @param out_path
 */
void GenerateGo(const til::SerializableAST &ast, const std::string &out_path);

/**
 * Generate HTML Documentation for messages and services
 * @param ast
 * @param out_path
 */
void GenerateHTML(const til::SerializableAST &ast, const std::string &out_path);

int main(int argc, char**argv) {
  CLI::App app{"tell RPC code generator"};

  std::string file; // The path to the JSON AST
  std::string go_out; // The path to output generated go code
  std::string html_out; // The path to output generated HTML documentation

  // Set up app options
  app.add_option("-f,--file", file, "The full path to the compiled til json")->required(true);
  app.add_option("--go_out", go_out, "The full path for the generated Go output");
  app.add_option("--html_out", html_out, "The full path for the generated HTML output");
  CLI11_PARSE(app, argc, argv);

  // Read the AST and exit on failure
  til::SerializableAST ast;
  try {
    ast = ReadAST(file);
  } catch (std::exception &e) {
    std::cout << fmt::format("Encountered an error reading the input file: {}", e.what()) << std::endl;
    return 1;
  }

  // If no output options are selected then exit
  if (go_out.empty() && html_out.empty()) {
    std::cout << "No output format requested." << std::endl;
    return 1;
  }

  // If go is selected as an output option then generate the go code
  if (!go_out.empty()) {
    try {
      GenerateGo(ast, go_out);
    } catch (std::exception &e) {
      std::cout << fmt::format("Encountered an error generating Go: {}", e.what());
    }
  }

  // If HTML is selected as an ouput option then generate the HTML code
  if (!html_out.empty()) {
    try {
      GenerateHTML(ast, html_out);
    } catch (std::exception &e) {
      std::cout << fmt::format("Encountered an error generating HTML: {}", e.what());
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

void Generate(const til::SerializableAST &ast, const std::string &out_path, gen::Generator &generator) {
  std::ofstream file(out_path);
  if(!file.is_open()) {
    throw std::runtime_error(fmt::format("cannot open output file at {}", out_path));
  }

  file << generator.Generate(ast);
  file.flush();
  file.close();
}

void GenerateGo(const til::SerializableAST &ast, const std::string &out_path) {
  gen::GoGenerator generator;
  Generate(ast, out_path, generator);
}

void GenerateHTML(const til::SerializableAST &ast, const std::string &out_path) {
  gen::HTMLGenerator generator;
  Generate(ast, out_path, generator);
}

