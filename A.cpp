#include <functional>
#include <iterator>
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
