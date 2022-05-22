#pragma once

#include <utilsLib/ILoggingManager.h>
#include <utilsLib/LoggingSeverity.h>

#include <cassert>
#include <type_traits>
#include <chrono>

#define assert_return(cond, ...) if (!(cond)) { assert(false); return __VA_ARGS__; }
#define assert_continue(cond) if (!(cond)) { assert(false); continue; }

#define UTILS_LOG(sev, msg) utilsLib::ILoggingManager::getLogger()->logMessage(sev, msg, __FILE__, __FUNCTION__, __LINE__)

#ifdef _DEBUG
#define UTILS_LOG_TRACE(msg) UTILS_LOG(utilsLib::LoggingSeverity::Trace, msg)
#define UTILS_LOG_DEBUG(msg) UTILS_LOG(utilsLib::LoggingSeverity::Debug, msg)
#else
#define UTILS_LOG_TRACE(msg)
#define UTILS_LOG_DEBUG(msg)
#endif

#define UTILS_LOG_INFO(msg) UTILS_LOG(utilsLib::LoggingSeverity::Info, msg)
#define UTILS_LOG_WARN(msg) UTILS_LOG(utilsLib::LoggingSeverity::Warning, msg)
#define UTILS_LOG_ERROR(msg) UTILS_LOG(utilsLib::LoggingSeverity::Error, msg)
#define UTILS_LOG_FATAL(msg) UTILS_LOG(utilsLib::LoggingSeverity::Fatal, msg)

#define ENABLE_BITMASK_OPERATORS(EnumType)        \
template<>                                        \
struct utilsLib::EnableBitMaskOperators<EnumType> \
{                                                 \
    static const bool enable = true;              \
};

namespace utilsLib
{

std::string formatTimePoint(const std::chrono::system_clock::time_point& timePoint,
                            const std::string& format);

template <typename T>
typename std::underlying_type<T>::type underlying(T t)
{
  return static_cast<typename std::underlying_type<T>::type>(std::forward<T>(t));
}

template<typename EnumType>
struct EnableBitMaskOperators
{
  static const bool enable = false;
};

template<typename EnumType>
bool bitmask_has(EnumType mask, EnumType value)
{
  return (underlying<EnumType>(mask) & underlying<EnumType>(value)) > 0;
}

int rand32();
int rand32Between(int min, int max);
double randDouble32Between(double min, double max, double step);

}

template<typename EnumType>
typename std::enable_if<utilsLib::EnableBitMaskOperators<EnumType>::enable, EnumType>::type
operator |(EnumType lhs, EnumType rhs)
{
  return static_cast<EnumType>(
    utilsLib::underlying<EnumType>(lhs) |
    utilsLib::underlying<EnumType>(rhs));
}

template<typename EnumType>
typename std::enable_if<utilsLib::EnableBitMaskOperators<EnumType>::enable, EnumType>::type
operator &(EnumType lhs, EnumType rhs)
{
  return static_cast<EnumType>(
    utilsLib::underlying<EnumType>(lhs) &
    utilsLib::underlying<EnumType>(rhs));
}
