#include "utilsLib/NullLoggingManager.h"

namespace utilsLib
{

NullLoggingManager::~NullLoggingManager() = default;

LoggingSeverity NullLoggingManager::getLogLevelMinimumSeverity() const
{
  return LoggingSeverity::Info;
}

void NullLoggingManager::setLogLevelMinimumSeverity(LoggingSeverity /*severity*/)
{
}

void NullLoggingManager::logMessage(LoggingSeverity /*severity*/, const std::string& /*message*/, const std::string& /*file*/,
                                    const std::string& /*function*/, int /*line*/) const
{
}

void NullLoggingManager::addLoggingStrategy(const std::shared_ptr<ILoggingStrategy>& /*strategy*/)
{
}

}
