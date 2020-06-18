#include "Strcase.h"

#include <algorithm>
#include <sstream>
#include <vector>

/**
 * Split the string s at any sequence of numbers. Join the resulting string vector delimited by spaces
 * @param s
 * @return
 */
std::string AddWordBoundariesToNumbers(std::string s);

/**
 * If the string s is not empty push it to the back of the vector v
 * @param s
 * @param v
 */
void PushIfNotEmpty(std::string &s, std::vector<std::string> &v);

/**
 * Join the vector parts delimited by the string delim.
 *
 * @param parts
 * @param delim
 */
std::string Join(std::vector<std::string> parts, const std::string& delim);

/**
 * The following LTrim, RTrim, Trim solution taken from this stack overflow answer
 * https://stackoverflow.com/a/25385766/158413
 */

static const char* ws = " \t\n\r\f\v";
/**
 * Trim right whitespace from s
 * @param s
 * @return
 */
inline std::string & RTrim(std::string &s);

/**
 * Trim left whitespace from s
 * @param s
 * @return
 */
inline std::string & LTrim(std::string &s);

/**
 * Trim left and right whitespace from s
 * @param s
 * @return
 */
inline std::string &Trim(std::string &s);

/**
 * Returns true if S is an acronym which should retain its upper casing
 * @param s
 * @return
 */
bool UppercaseAcronym(std::string &s);

bool UppercaseAcronym(std::string &s)  {
  return s == "ID";
}

inline std::string & RTrim(std::string &s) {
  s.erase(s.find_last_not_of(ws) + 1);
  return s;
}

inline std::string & LTrim(std::string &s) {
  s.erase(0, s.find_first_not_of(ws));
  return s;
}

inline std::string & Trim(std::string &s) {
  return RTrim(LTrim(s));
}


std::string gen::ToSnake(std::string s) {
  const char delimiter = '_';
  s = AddWordBoundariesToNumbers(s);
  s = Trim(s);
  std::string n;
  for(int i = 0; i < s.size(); i++) {
    char v = s.at(i);
    bool next_case_is_changed = false;
    if (i+1 < s.size()) {
      char next = s.at(i+1);
      bool v_is_cap = isupper(v);
      bool v_is_low = islower(v);
      bool next_is_cap = isupper(next);
      bool next_is_low = islower(next);
      next_case_is_changed = (v_is_cap && next_is_low) || (v_is_low && next_is_cap);
    }
    if (i > 0 && n.at(n.size()-1) != delimiter && next_case_is_changed) {
      if(isupper(v)) {
        n += delimiter;
        n += tolower(v);
      } else if (islower(v)) {
        n += v;
        n += delimiter;
      }
    } else if (v == ' ' || v == delimiter || v == '-') {
      n += delimiter;
    } else {
      n += tolower(v);
    }
  }
  return n;
}

std::string gen::ToCamel(std::string s, bool upper_first) {
  if (s.empty()) {
    return s;
  }
  if (UppercaseAcronym(s)) {
    std::for_each(s.begin(), s.end(), [](char & c) {
      c = tolower(c);
    });
  }
  if (!upper_first) {
    s.at(0) = tolower(s.at(0));
  }
  s = AddWordBoundariesToNumbers(s);
  s = Trim(s);

  std::string n;
  bool cap_next = upper_first;
  for (char & v : s) {
    if (!isupper(v)) {
      if (isdigit(v)) {
        n += v;
      } else if (islower(v)) {
        n += cap_next ? toupper(v) : v;
      }
    } else {
      n += v;
    }
    cap_next = v == '_' || v == '-' || v == ' ' || v == '.';
  }
  return n;
}

std::string AddWordBoundariesToNumbers(std::string s) {
  int i = 0;
  std::vector<std::string> parts;
  std::string current;

  while (i < s.size()) {
    if(isdigit(s.at(i))) {
      PushIfNotEmpty(current, parts);
      current = "";
      while(i < s.size() && isdigit(s.at(i))) {
        current += s.at(i);
        i++;
      }
      PushIfNotEmpty(current, parts);
      current = "";
    } else {
      current += s.at(i);
      i++;
    }
  }
  PushIfNotEmpty(current, parts);
  return Join(parts, " ");
}


void PushIfNotEmpty(std::string &s, std::vector<std::string> &v) {
  if (s.empty()) {
    return;
  }
  v.push_back(s);
}

std::string Join(std::vector<std::string> parts, const std::string& delim) {
  std::ostringstream joined;
  if (parts.empty()) {
    return joined.str();
  }

  for(int i = 0; i < parts.size() -1; i++) {
    std::string &s = parts.at(i);
    joined << s << delim;
  }
  joined << parts.at(parts.size() -1);
  return joined.str();
}
