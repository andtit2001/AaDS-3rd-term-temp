#include <bitset>
#include <string>
#include <string_view>
#include <vector>

template <class CharT = char>
class PrefixFuncToStringConverter {
  std::vector<size_t> prefix_function_;
  std::bitset<26> banned_letters_;
  std::basic_string<CharT> generated_string_;

 public:
  std::basic_string_view<CharT> GetString() const noexcept;
  void AppendPrefixFuncValue(size_t val);
};

////////////////////////////////////////////////////////////////////////////////

template <class CharT>
std::basic_string_view<CharT> PrefixFuncToStringConverter<CharT>::GetString()
    const noexcept {
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
  // If `val == 0` then we need to insert lexicographically minimal letter
  // that does not "break" that zero.
  for (val = prev_value;; val = prefix_function_[val - 1]) {
    // Mark each letter that can "break" zero
    banned_letters_[generated_string_[val] - 'a'] = true;
    if (val == 0) break;
  }
  for (size_t i = 0; i < 26; ++i)
    if (!banned_letters_[i]) {
      generated_string_.push_back('a' + static_cast<CharT>(i));
      banned_letters_.reset();
      break;
    }
}
