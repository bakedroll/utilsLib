#pragma once

#include <utilsLib/LoggingSeverity.h>

#include <string>

namespace utilsLib
{

class ILoggingStrategy
{
public:
  ILoggingStrategy() = default;
  virtual ~ILoggingStrategy() = default;

  virtual void deliverMessage(LoggingSeverity severity, const std::string& message) = 0;

};

}
