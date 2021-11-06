#include "utilsLib/Utils.h"

#include <ctime>

namespace utilsLib
{

std::string formatTimePoint(const std::chrono::system_clock::time_point& timePoint,
                            const std::string& format)
{
  const auto time = std::chrono::system_clock::to_time_t(timePoint);

  auto t = std::localtime(&time);

  char buffer[128];
  strftime(buffer, sizeof(buffer), format.c_str(), t);

  return buffer;
}

int rand32()
{
  return (rand() << 16) + rand();

}
int rand32Between(int min, int max)
{
  const auto range = max - min;
  return (rand32() % range) + min;
}

double randDouble32Between(double min, double max, double step)
{
  const auto range = static_cast<int>((max - min) / step);
  return (static_cast<double>(rand32() % range) * step) + min;
}

}
