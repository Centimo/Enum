// author: Filip Roséen <filip.roseen@gmail.com>
// source: http://b.atch.se/posts/constexpr-meta-container

#ifndef ATCH_META_COUNTER_HPP
#define ATCH_META_COUNTER_HPP

#include <cstddef>


namespace atch { namespace {

  template <std::size_t index_t>
  struct Global_tag
  {
    static constexpr size_t index = index_t;
  };

  template<class Tag>
  struct Meta_counter {
    using size_type = std::size_t;

    template<size_type N>
    struct ident {
      friend constexpr size_type adl_lookup (ident<N>);
      static constexpr size_type value = N;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    template<class Ident>
    struct writer {
      friend constexpr size_type adl_lookup (Ident) {
        return Ident::value;
      }

      static constexpr size_type value = Ident::value;
    };
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    template<size_type N, int = adl_lookup (ident<N> {})>
    static constexpr size_type value_reader (int, ident<N>) {
      return N;
    }

    template<size_type N>
    static constexpr size_type value_reader (float, ident<N>, size_type R = value_reader (0, ident<N-1> ())) {
      return R;
    }

    static constexpr size_type value_reader (float, ident<0>) {
      return 0;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    template<size_type Max = 64>
    static constexpr size_type value (size_type R = value_reader (0, ident<Max> {})) {
      return R;
    }

    template<size_type N = 1, class H = Meta_counter>
    static constexpr size_type next (size_type R = writer<ident<N + H::value ()>>::value) {
      return R;
    }
  };

  struct Global_count;
  using Global_meta_counter = Meta_counter<Global_count>;



  //template<template<class> class TT, class C = atch::Meta_counter<class GlobalCount>>
  //using create_new = TT<Global_tag<C::next ()>>;

  template <template<size_t> class TT, size_t index = Global_meta_counter::next ()>
  using Create_new = TT<index>;

  template <template<size_t> class TT, typename Counter_type, size_t index = Counter_type::next ()>
  using Create_new_by_counter = TT<index>;

  //template <template<class> class TT, class Tag_class = Create_new<Global_tag>>
  //using Create_new_meta = TT<Tag_class>;
}}



#endif /* include guard */
