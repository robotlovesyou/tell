#include "Compiler.h"

#include <iostream>
#include <memory>
#include <utility>

#include "nlohmann/json.hpp"

#include "FileCursor.h"
#include "Lexer.h"
#include "Parser.h"
#include "SerializableAST.h"

using nlohmann::json;

Compiler::Compiler(std::string f, std::string o, std::shared_ptr<til::ErrorReporter>  error_reporter, bool pretty)
    : file_(std::move(f)), out_(std::move(o)), error_reporter_(std::move(error_reporter)), pretty_(pretty) {
}

bool Compiler::Compile() {
  try {
    auto file_cursor = std::make_unique<til::FileCursor>(file_);
    auto out_file = OpenOutFile();
    auto lexer = std::make_unique<til::Lexer>(std::move(file_cursor), error_reporter_);
    til::Parser parser(std::move(lexer), error_reporter_);

    auto ast = parser.Parse();
    if (error_reporter_->has_errors()) {
      return false;
    }

    auto sast = ast->ToSerializable();
    json j(sast);

    auto output = pretty_ ? j.dump(4) : j.dump();
    out_file << output;
    out_file.flush();
    out_file.close();

    return true;
  } catch (const OutputException &oe) {
    std::cout << fmt::format("Error opening output file: {}", oe.what()) << std::endl;
  } catch (const til::FileCursor::FileCursorException &fce) {
    std::cout << fmt::format("Error opening input file: {}", fce.what());
  } catch (const std::exception &e) {
    std::cout << fmt::format("Unexpected error compiling file: {}", e.what());
  }
  return false;
}

std::ofstream Compiler::OpenOutFile() {
  std::ofstream out_file(out_);
  if (!out_file) {
    throw OutputException(fmt::format("cannot open file {} for writing", out_));
  }
  return out_file;
}

Compiler::OutputException::OutputException(std::string reason) : reason_(std::move(reason)) {}

const char *Compiler::OutputException::what() const noexcept {
  return reason_.c_str();
}
