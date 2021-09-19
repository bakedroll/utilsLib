#pragma once

#include <utilsLib/ILoggingManager.h>

#include <vector>
#include <mutex>

namespace utilsLib
{

class LoggingManager : public ILoggingManager
{
  friend class ILoggingManager;

public:
  ~LoggingManager() override;

  LoggingSeverity getLogLevelMinimumSeverity() const override;
  void setLogLevelMinimumSeverity(LoggingSeverity severity) override;

  void logMessage(LoggingSeverity severity, const std::string& message,
                  const std::string& file, const std::string& function,
                  int line = 0) const override;

  void addLoggingStrategy(const std::shared_ptr<ILoggingStrategy>& strategy) override;

protected:
  LoggingManager();

private:
  using LoggingStrategyList = std::vector<std::shared_ptr<ILoggingStrategy>>;

  LoggingSeverity     m_minSeverity;
  LoggingStrategyList m_loggingStrategies;

  mutable std::mutex m_mutex;

};

}