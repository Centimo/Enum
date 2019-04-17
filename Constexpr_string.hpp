//
// Created by centimo on 17.04.19.
//

#ifndef TEST_CONSTEXPR_STRING_HPP
#define TEST_CONSTEXPR_STRING_HPP


#include <utility>


constexpr size_t length(const char* str)
{
  return (*str == 0) ? 0 : length(str + 1) + 1;
}

template <char... symbols>
struct Constexpr_string
{
  static constexpr char value[sizeof... (symbols)] = {symbols...};
  static constexpr size_t size = sizeof... (symbols);

  using Char_sequence = std::integer_sequence<char, symbols...>;
};

// based on GCC extension, NOT STANDARD
template <typename Char_type = char, Char_type... symbols>
inline constexpr Constexpr_string<symbols...> operator ""_const()
{
  return Constexpr_string<symbols...>();
}

// Standard-correct variant

template <typename Lambda_type, std::size_t... indexes>
constexpr static auto get_constexpr_string(Lambda_type function, std::index_sequence<indexes...>)
{
  return Constexpr_string<function()[indexes]...>();
}

template <std::size_t N,
          typename Lambda_type,
          typename Indices = std::make_index_sequence<N>
         >
constexpr static auto get_constexpr_string(Lambda_type function)
{
  return get_constexpr_string(function, Indices{});
}

template <typename Lambda_type>
static constexpr auto get_constexpr_string(Lambda_type function)
{
  return get_constexpr_string<length(function())>(function);
}

#endif //TEST_CONSTEXPR_STRING_HPP
