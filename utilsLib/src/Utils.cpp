#include "utilsLib/Utils.h"

namespace utilsLib
{

std::string formatTimePoint(const std::chrono::system_clock::time_point& timePoint,
                            const std::string& format)
{
  const auto time = std::chrono::system_clock::to_time_t(timePoint);

  tm t;
  if (localtime_s(&t, &time) == EDOM)
  {
    return "";
  }

  char buffer[128];
  strftime(buffer, sizeof(buffer), format.c_str(), &t);

  return buffer;
}

}
