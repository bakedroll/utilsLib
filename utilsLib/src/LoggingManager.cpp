#include "utilsLib/LoggingManager.h"

#include <ctime>
#include <chrono>
#include <cassert>

namespace utilsLib
{

std::string severityToString(LoggingSeverity severity)
{
  switch(severity)
  {
  case LoggingSeverity::Trace:
    return "TRACE";
  case LoggingSeverity::Debug:
    return "DEBUG";
  case LoggingSeverity::Info:
    return "INFO";
  case LoggingSeverity::Warning:
    return "WARNING";
  case LoggingSeverity::Error:
    return "ERROR";
  case LoggingSeverity::Fatal:
    return "FATAL";
  default:
    assert(false);
    break;
  }

  return "";
}

std::string formatCurrentDateTime(const std::string& format)
{
  using sc        = std::chrono::system_clock;
  const auto time = sc::to_time_t(sc::now());

  char buffer[128];
  strftime(buffer, sizeof(buffer), format.c_str(), std::localtime(&time));

  return buffer;
}

LoggingManager::~LoggingManager() = default;

void LoggingManager::setLogLevelMinimumSeverity(LoggingSeverity severity)
{
  m_minSeverity = severity;
}

void LoggingManager::logMessage(LoggingSeverity severity, const std::string& message)
{
  if (severity < m_minSeverity)
  {
    return;
  }

  const auto dateTimeString = formatCurrentDateTime("%Y-%m-%d %H:%M:%S");
  const auto severityString = severityToString(severity);

  const std::string text = "[" + dateTimeString + " " + severityString + "] " + message;

  std::lock_guard<std::mutex> lock(m_mutex);
  for (const auto& strategy : m_loggingStrategies)
  {
    strategy->deliverMessage(severity, text);
  }
}

void LoggingManager::addLoggingStrategy(const std::shared_ptr<ILoggingStrategy>& strategy)
{
  m_loggingStrategies.emplace_back(strategy);
}

LoggingManager::LoggingManager()
  : ILoggingManager()
  , m_minSeverity(LoggingSeverity::Debug)
{
}

std::unique_ptr<ILoggingManager> ILoggingManager::m_instance;

}
