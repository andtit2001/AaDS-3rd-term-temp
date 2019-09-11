#include <functional>
#include <iterator>
#include <string_view>
#include <vector>

template <class RAIter, class Equiv = std::equal_to<
                            typename std::iterator_traits<RAIter>::value_type>>
std::vector<size_t> PrefixFunction(RAIter begin, RAIter end,
                                   Equiv eq = Equiv()) {
  std::vector<size_t> prefix_func(std::distance(begin, end));
  size_t current_value = 0;
  for (RAIter it = std::next(begin); it != end; ++it) {
    while (true) {
      if (eq(*it, *std::next(begin, current_value))) {
        prefix_func[std::distance(begin, it)] = ++current_value;
        break;
      }
      if (current_value == 0) break;
      current_value = prefix_func[current_value - 1];
    }
  }
  return prefix_func;
}

std::vector<size_t> KnuthMorrisPratt(std::string_view pattern,
                                     std::string_view text) {
  auto prefix_function = PrefixFunction(pattern.cbegin(), pattern.cend());
  size_t accumulated_length = 0;
  std::vector<size_t> occurrences;
  for (size_t i = 0; i < text.size(); ++i) {
    while (true) {
      if (text[i] == pattern[accumulated_length]) {
        ++accumulated_length;
        break;
      }
      if (accumulated_length == 0) break;
      accumulated_length = prefix_function[accumulated_length - 1];
    }
    if (accumulated_length == pattern.size()) {
      occurrences.push_back(i);
      accumulated_length = prefix_function[accumulated_length - 1];
    }
  }
  return occurrences;
}
