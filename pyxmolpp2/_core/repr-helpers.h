#include <string>
namespace pyxmolpp::v1 {

template <typename T> std::string to_string_3_elements(T& self) {
  std::string result;
  if (self.size() <= 3) {
    if (self.size() > 0) {
      result = to_string(self[0]);
      for (auto& a : self.slice(1, {})) {
        result += ", " + to_string(a);
      }
    }
  } else {
    result = to_string(self[0]) + ", " + to_string(self[1]) + ", ... , " + to_string(self[self.size() - 1]);
  }
  return result;
}

} // namespace pyxmolpp::v1