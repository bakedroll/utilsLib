#include "utilsLib/StdOutLoggingStrategy.h"

#include <iostream>

namespace utilsLib
{

void StdOutLoggingStrategy::deliverMessage(LoggingSeverity severity, const std::string& message)
{
  auto& stream = (severity >= LoggingSeverity::Warning) ? std::cerr : std::cout;
  stream << message << std::endl;
}

}
