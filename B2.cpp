#include <bitset>
#include <iostream>
#include <string>
#include <vector>

std::string ConvertZFuncToString(const std::vector<size_t>& z_func) {
  std::bitset<26> banned_letters;
  std::string generated_string;
  if (z_func.empty()) return generated_string;
  generated_string.reserve(z_func.size());
  generated_string.push_back('a');

  std::vector<size_t> z_block_stack;
  for (size_t i = 1; i < z_func.size(); ++i) {
    if (i + z_func[i] < generated_string.size()) continue;
    if (i + z_func[i] == generated_string.size()) {
      z_block_stack.push_back(i);
      if (z_func[i] != 0) continue;

      for (; !z_block_stack.empty(); z_block_stack.pop_back())
        banned_letters[generated_string[z_func[z_block_stack.back()]] - 'a'] =
            true;
      for (size_t j = 0; j < 26; ++j)
        if (!banned_letters[j]) {
          generated_string.push_back('a' + static_cast<char>(j));
          banned_letters.reset();
          break;
        }
    } else {
      size_t current_z_block = i;
      size_t current_pos = generated_string.size();
      while (current_pos < current_z_block + z_func[current_z_block]) {
        if (current_pos + z_func[current_pos] >
            current_z_block + z_func[current_z_block]) {
          current_z_block = current_pos;
        }
        generated_string.push_back(
            generated_string[current_pos - current_z_block]);
        ++current_pos;
      }

      z_block_stack.clear();
      z_block_stack.push_back(i = current_z_block);
    }
  }
  return generated_string;
}

int main() {
  size_t value;
  std::vector<size_t> z_func;
  while (std::cin >> value)
    z_func.push_back(value);
  z_func[0] = 0;
  std::cout << ConvertZFuncToString(z_func) << std::endl;
}
