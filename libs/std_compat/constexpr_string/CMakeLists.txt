
# constexpr std::string compatibility

check_cxx_source_compiles("
#include <string>
constexpr size_t len(const std::string& s) { return s.size(); }
int main() { constexpr size_t n = len(\"hello\"); return 0; }
" HAS_STD_CONSTEXPR_STRING)

if(HAS_STD_CONSTEXPR_STRING)
  target_compile_definitions(std_compat INTERFACE HAS_STD_CONSTEXPR_STRING)
endif(HAS_STD_CONSTEXPR_STRING)
