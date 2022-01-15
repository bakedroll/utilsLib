#pragma once

#include <utilsLib/ILoggingManager.h>

namespace utilsLib
{

class NullLoggingManager : public ILoggingManager
{
  friend class ILoggingManager;

public:
  ~NullLoggingManager() override;

  LoggingSeverity getLogLevelMinimumSeverity() const override;
  void setLogLevelMinimumSeverity(LoggingSeverity severity) override;

  void logMessage(LoggingSeverity severity, const std::string& message,
    const std::string& file, const std::string& function,
    int line = 0) const override;

  void addLoggingStrategy(const std::shared_ptr<ILoggingStrategy>& strategy) override;

protected:
  NullLoggingManager() = default;

};

}
