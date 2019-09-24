#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

template <class StringRAIter, class FuncRAIter,
          class CharT = typename std::iterator_traits<StringRAIter>::value_type,
          class Equiv = std::equal_to<CharT>>
size_t GetNextPrefixFunctionValue(StringRAIter str_begin, FuncRAIter func_begin,
                                  size_t current_value, CharT ch,
                                  Equiv eq = Equiv()) {
  static_assert(
      std::is_same_v<typename std::iterator_traits<FuncRAIter>::value_type,
                     size_t>);

  while (true) {
    if (eq(ch, *std::next(str_begin, current_value))) return current_value + 1;
    if (current_value == 0) break;
    current_value = *std::next(func_begin, current_value - 1);
  }
  return 0;
}

template <class RAIter, class Equiv = std::equal_to<
                            typename std::iterator_traits<RAIter>::value_type>>
std::vector<size_t> PrefixFunction(RAIter begin, RAIter end,
                                   Equiv eq = Equiv()) {
  std::vector<size_t> prefix_func;
  if (begin == end) return prefix_func;
  prefix_func.reserve(std::distance(begin, end));
  prefix_func.push_back(0);
  for (RAIter it = std::next(begin); it != end; ++it)
    prefix_func.push_back(GetNextPrefixFunctionValue(
        begin, prefix_func.cbegin(), prefix_func.back(), *it, eq));
  return prefix_func;
}

std::vector<size_t> KnuthMorrisPratt(std::string_view pattern,
                                     std::string_view text) {
  auto prefix_function = PrefixFunction(pattern.cbegin(), pattern.cend());
  size_t accumulated_length = 0;
  std::vector<size_t> occurrences;
  for (size_t i = 0; i < text.size(); ++i) {
    accumulated_length =
        GetNextPrefixFunctionValue(pattern.cbegin(), prefix_function.cbegin(),
                                   accumulated_length, text[i]);
    if (accumulated_length == pattern.size()) occurrences.push_back(i);
  }
  return occurrences;
}

int main() {
  std::string pattern, text;
  std::getline(std::cin, pattern);
  std::getline(std::cin, text);
  for (auto num : KnuthMorrisPratt(pattern, text))
    std::cout << num + 1 - pattern.size() << ' ';
}
