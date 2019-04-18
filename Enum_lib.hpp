//
// Created by centimo on 05.04.19.
//

#ifndef TEST_ENUM_LIB_HPP
#define TEST_ENUM_LIB_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>

#include "Stateful_meta_container/Meta_counter.hpp"
#include "Stateful_meta_container/meta_list.hpp"
#include "Constexpr_string.hpp"


#define ADD_ENUM_VALUE(name) static inline constexpr auto name = make_enum_value_const(#name##_const)

#define ADD_ENUM_VALUE_S(name) static inline constexpr auto name = make_enum_value_const([](){ return #name;})


namespace Enum_lib
{
  using Enum_lib_global_tag = atch::Create_new<atch::Global_tag>;
  using Enum_counter = atch::Meta_counter<Enum_lib_global_tag>;

  template <size_t enum_tag_index_t = Enum_counter::next()>
  struct Enum_tag : atch::Global_tag<Enum_lib_global_tag::index>
  {
    static constexpr size_t enum_tag_index = enum_tag_index_t;
  };


  template <size_t enum_type_id>
  class Enum_value
  {
  public:
    constexpr Enum_value(const size_t value_id, const char* value_name, size_t value_name_size) :
        _enum_value_id(value_id),
        _enum_value_name(value_name, value_name_size)
    { }

    constexpr operator size_t() const
    {
      return _enum_value_id;
    }

    constexpr size_t get_enum_id() const
    {
      return enum_type_id;
    }

    auto get_name() const
    {
      return _enum_value_name;
    }

    template <size_t second_enum_id>
    friend constexpr bool operator == (const Enum_value& first, const Enum_value<second_enum_id>& second)
    {
      static_assert(first.get_enum_id() == second_enum_id, "WARNING! Two different enums.");
      return size_t(first) == size_t(second) && first.get_name() == second.get_name();
    }


  private:
    size_t _enum_value_id;
    std::string_view _enum_value_name;
  };


  template <size_t enum_type_id, size_t enum_value_id, char... symbols>
  struct Enum_value_const
  {
    using Name = Constexpr_string<symbols...>;

  public:
    Enum_value_const() = default;

    constexpr operator size_t() const
    {
      return enum_value_id;
    }

    constexpr operator Enum_value<enum_type_id>() const
    {
      return Enum_value<enum_type_id>(enum_value_id, Name::value, Name::size);
    }
  };


// Standard-correct variant

  template <std::size_t enum_type_id, std::size_t enum_value_id, char... symbols>
  constexpr auto get_enum_value_const(Constexpr_string<symbols...>)
  {
    return Enum_value_const<enum_type_id, enum_value_id, symbols...>();
  }


  template <size_t enum_type_id = Enum_counter::next()>
  class Enum
  {

  private:

    using Counter = atch::Meta_counter<Enum_tag<enum_type_id> >;
    using Enum_list = atch::meta_list<Enum_tag<enum_type_id> >;


  private:

    template <size_t... indexes>
    static std::vector<Enum_value<enum_type_id> > make_vector_of_enum_values_helper(std::index_sequence<indexes...>)
    {
      return { typename Enum<enum_type_id>::Enum_list::template value<>::template at<indexes>::result()...};
    }

    template <size_t counter_value = Counter::value(), typename Indexes = std::make_index_sequence<counter_value> >
    static auto make_vector_of_enum_values()
    {
      return make_vector_of_enum_values_helper(Indexes{});
    }

    static const inline std::vector<Enum_value<enum_type_id> > _values = make_vector_of_enum_values();


    static std::unordered_map<std::string, size_t> make_values_map()
    {
      std::unordered_map<std::string, size_t> result;
      for (size_t i = 0; i < _values.size(); ++i)
      {
        result.emplace(std::make_pair(_values[i].get_name(), i));
      }

      return result;
    }

    static inline std::unordered_map<std::string, size_t> _values_ids_by_names = make_values_map();


  public:

    static Enum_value<enum_type_id> get_value_by_name(const std::string& name)
    {
      return _values[_values_ids_by_names.at(name)];
    }

    static std::optional<Enum_value<enum_type_id> > find_value_by_name(const std::string& name)
    {
      auto search_result = _values_ids_by_names.find(name);
      if (search_result == _values_ids_by_names.end())
      {
        return std::nullopt;
      }

      return _values[_values_ids_by_names.at(name)];
    }

    // Standard-correct variant
    template <typename Lambda_type>
    static constexpr Enum_value<enum_type_id> make_enum_value_const(Lambda_type function)
    {
      constexpr auto name = get_constexpr_string(function);
      constexpr size_t counter_value = Counter::next() - 1;
      Enum_list::template push<decltype(get_enum_value_const<enum_type_id, counter_value>(name))>();
      return Enum_value<enum_type_id>(counter_value, name.value, name.size);
    }

    // gcc extension
    template <char... symbols>
    static constexpr Enum_value<enum_type_id> make_enum_value_const(Constexpr_string<symbols...>)
    {
      constexpr size_t counter_value = Counter::next() - 1;
      Enum_list::template push<Enum_value_const<enum_type_id, counter_value, symbols...> >();
      return Enum_value<enum_type_id>(counter_value,
                                      Constexpr_string<symbols...>::value,
                                      Constexpr_string<symbols...>::size);
    }

    const auto begin() const
    {
      return _values.begin();
    }

    const auto end() const
    {
      return _values.end();
    }
  };
}


#endif //TEST_ENUM_LIB_HPP
