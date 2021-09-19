#pragma once

#include <cassert>
#include <type_traits>

#define assert_return(cond, ...) if (!(cond)) { assert(false); return __VA_ARGS__; }
#define assert_continue(cond) if (!(cond)) { assert(false); continue; }

namespace utilsLib
{

template <typename T>
typename std::underlying_type<T>::type underlying(T t)
{
  return static_cast<typename std::underlying_type<T>::type>(std::forward<T>(t));
}

}