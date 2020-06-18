#ifndef TELL_TESTS_CASETESTHELPERS_H_
#define TELL_TESTS_CASETESTHELPERS_H_
#include <string>
#include <tuple>

static std::tuple<std::string, std::string> case_test(const std::string &before, const std::string &after) {
  return std::tuple<std::string, std::string>(before, after);
}
#endif //TELL_TESTS_CASETESTHELPERS_H_
