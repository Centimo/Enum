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

  auto temp_value_2 = My_enum::first;
  temp_value_2 = My_enum::second;
  auto temp_value = My_enum::second;
  auto temp_value_3 = My_enum_2::second;
  auto temp_value_name = temp_value.get_name();
  std::cout << temp_value_name << std::endl;


  temp_value = My_enum::get_value_by_name("asfasf");  //error: temp_value is constexpr
  auto new_temp = My_enum_2::get_value_by_name("second");

  std::cout << new_temp.get_name() << std::endl;


  switch(temp_value)
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



  return 0;
}