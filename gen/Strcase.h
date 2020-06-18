#ifndef TELL_GEN_STRCASE_H_
#define TELL_GEN_STRCASE_H_

#include <string>
#include <cctype>

/**
 * This code is a partial port of the Golang strcase library.
 * https://github.com/iancoleman/strcase
 */
namespace gen {
  /**
   * Convert s to snake_case
   * @param s
   * @return
   */
 std::string ToSnake(std::string s);


 /**
  * Convert s to CamelCase.
  * If upper_first is false convert s to lowerCamelCase
  * @param s
  * @param upper_first
  * @return
  */
 std::string ToCamel(std::string s, bool upper_first = true);
}

#endif //TELL_GEN_STRCASE_H_
