#pragma once

#include <utilsLib/ILoggingStrategy.h>

namespace utilsLib
{

class StdOutLoggingStrategy : public ILoggingStrategy
{
public:
  void deliverMessage(LoggingSeverity severity, const std::string& message) override;
};

}