#include "catch2/catch.hpp"
#include "../compiler/Call.h"
#include "../compiler/Argument.h"
#include "ParserTestHelpers.h"

TEST_CASE("Call constructor"){
  auto ast = test_ast();
  auto argument = std::make_unique<til::Argument>("my_arg", ast);
  auto returns = std::make_unique<til::Argument>("my_arg", ast);
  CHECK_NOTHROW(til::Call("call_name", test_dcc(), std::move(argument), std::move(returns)));
}

TEST_CASE("Call.name") {
  auto ast = test_ast();
  auto argument = std::make_unique<til::Argument>("my_arg", ast);
  auto returns = std::make_unique<til::Argument>("my_arg", ast);
  til::Call call("call_name", test_dcc(), std::move(argument), std::move(returns));
  CHECK(call.name() == "call_name");
}

TEST_CASE("Call.argument") {
  auto ast = test_ast();
  auto argument = std::make_unique<til::Argument>("my_argument", ast);
  auto returns = std::make_unique<til::Argument>("my_return", ast);
  til::Call call("call_name", test_dcc(), std::move(argument), std::move(returns));
  CHECK(call.argument().name() == "my_argument");
}

TEST_CASE("Call.returns") {
  auto ast = test_ast();
  auto argument = std::make_unique<til::Argument>("my_argument", ast);
  auto returns = std::make_unique<til::Argument>("my_return", ast);
  til::Call call("call_name", test_dcc(), std::move(argument), std::move(returns));
  CHECK(call.returns().name() == "my_return");
}

TEST_CASE("Call.doc") {
  auto ast = test_ast();
  auto argument = std::make_unique<til::Argument>("my_argument", ast);
  auto returns = std::make_unique<til::Argument>("my_return", ast);
  auto doc = std::make_unique<til::DocCommentContext>();
  doc->append("A comment");
  til::Call call("call_name", std::move(doc), std::move(argument), std::move(returns));
  CHECK(call.doc().content() == "A comment");
}
