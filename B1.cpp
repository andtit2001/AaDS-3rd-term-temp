#include <bitset>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

template <class CharT = char>
class PrefixFuncToStringConverter {
  std::vector<size_t> prefix_function_;
  std::basic_string<CharT> generated_string_;

 public:
  std::basic_string_view<CharT> GetStringView() const noexcept;
  std::basic_string<CharT> GetString() const;
  void AppendPrefixFuncValue(size_t val);
  template <class InputIter>
  void AppendPrefixFuncValues(InputIter begin, InputIter end);
};

////////////////////////////////////////////////////////////////////////////////

int main() {
  PrefixFuncToStringConverter generator;
  generator.AppendPrefixFuncValues(std::istream_iterator<size_t>(std::cin),
                                   std::istream_iterator<size_t>());
  std::cout << generator.GetStringView() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

template <class CharT>
std::basic_string_view<CharT>
PrefixFuncToStringConverter<CharT>::GetStringView() const noexcept {
  return generated_string_;
}

template <class CharT>
std::basic_string<CharT> PrefixFuncToStringConverter<CharT>::GetString() const {
  return generated_string_;
}

template <class CharT>
void PrefixFuncToStringConverter<CharT>::AppendPrefixFuncValue(size_t val) {
  if (prefix_function_.empty()) {
    prefix_function_.push_back(0);  // The first value of prefix function
    generated_string_.push_back('a');
    return;
  }
  size_t prev_value = prefix_function_.back();
  prefix_function_.push_back(val);
  if (val > 0) {
    generated_string_.push_back(generated_string_[val - 1]);
    return;
  }

  std::bitset<26> banned_letters;
  // If `val == 0` then we need to insert lexicographically minimal letter
  // that does not "break" that zero.
  for (val = prev_value;; val = prefix_function_[val - 1]) {
    // Mark each letter that can "break" zero
    banned_letters[generated_string_[val] - 'a'] = true;
    if (val == 0) break;
  }
  for (size_t i = 0; i < 26; ++i)
    if (!banned_letters[i]) {
      generated_string_.push_back('a' + static_cast<CharT>(i));
      banned_letters.reset();
      break;
    }
}

template <class CharT>
template <class InputIter>
void PrefixFuncToStringConverter<CharT>::AppendPrefixFuncValues(InputIter begin,
                                                                InputIter end) {
  static_assert(
      std::is_same_v<typename std::iterator_traits<InputIter>::value_type,
                     size_t>);
  for (auto it = begin; it != end; ++it) AppendPrefixFuncValue(*it);
}
