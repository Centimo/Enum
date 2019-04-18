#include <iostream>
#include <type_traits>

#include "Enum_lib.hpp"


struct My_enum : public Enum_lib::Enum<>
{
  ADD_ENUM_VALUE_S(first);
  ADD_ENUM_VALUE_S(second);
  ADD_ENUM_VALUE_S(third);
  ADD_ENUM_VALUE_S(asfasf);
  ADD_ENUM_VALUE_S(asfasfasd);
};


struct My_enum_2 : public Enum_lib::Enum<>
{
  ADD_ENUM_VALUE(first);
  ADD_ENUM_VALUE(second);
  ADD_ENUM_VALUE(third);
};

int main()
{
  std::cout << "Hello, World!" << std::endl;

  auto temp_value = My_enum::second;
  std::cout << temp_value.get_name() << std::endl;

  auto new_temp = My_enum_2::get_value_by_name("second");
  std::cout << new_temp.get_name() << std::endl;

  auto search_result = My_enum::find_value_by_name("asfasfasd");
  if (search_result)
  {
    std::cout << search_result.value().get_name() << std::endl;
  }


  for (const auto& value : My_enum{})
  {
    switch (value)
    {
      case My_enum::first:
      {
        std::cout << "First" << std::endl;
        break;
      }
      case My_enum::second:
      {
        std::cout << "Second" << std::endl;
        break;
      }
      case My_enum::third:
      {
        std::cout << "Third" << std::endl;
        break;
      }
    }
  }

  return 0;
}