//
// Created by centimo on 13.10.2019.
//

#pragma once

#include <variant>

#include "Stateful_meta/meta_list.hpp"


namespace Meta_base_inner
{
  template <typename List_type, typename Appended_type, typename Counter>
  constexpr size_t append_to_list()
  {
    if constexpr (std::is_void<Appended_type>::value)
    {
      return Counter::value();
    }
    else
    {
      List_type::template push<Appended_type>();
      return Counter::next();
    }
  }
}

template <typename Base_type, typename Derived_type>
class Meta_base
{
private:
  using Derived_types_list = atch::meta_list<Meta_base<Base_type, void> > ;
  using Counter = atch::Meta_counter<Meta_base<Base_type, void> >;

  template <typename List_type, size_t... indexes>
  static constexpr auto inheritors_helper(std::index_sequence<indexes...>)
  {
    return std::variant<std::monostate, typename List_type::template value<>::template at<indexes>::result...>();
  }

public:
  template <
      typename List_type = Derived_types_list,
      typename Indexes = std::make_index_sequence<Meta_base_inner::append_to_list<Derived_types_list, Derived_type, Counter>()>
  >
  using Inheritors = decltype(inheritors_helper<List_type>(Indexes{}));
};