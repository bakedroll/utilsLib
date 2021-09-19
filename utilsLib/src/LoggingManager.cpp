#include "utilsLib/LoggingManager.h"
#include "utilsLib/Utils.h"

#include <chrono>
#include <cassert>
#include <filesystem>

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
  return formatTimePoint(std::chrono::system_clock::now(), format);
}

std::string getFilenameFromPath(const std::string& path)
{
  const std::filesystem::path p(path);
  return p.filename().string();
}

LoggingManager::~LoggingManager() = default;

LoggingSeverity LoggingManager::getLogLevelMinimumSeverity() const
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_minSeverity;
}

void LoggingManager::setLogLevelMinimumSeverity(LoggingSeverity severity)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_minSeverity = severity;
}

void LoggingManager::logMessage(LoggingSeverity severity, const std::string& message,
                                const std::string& file, const std::string& function,
                                int line) const
{
  if (severity < getLogLevelMinimumSeverity())
  {
    return;
  }

  const std::string text = "[" +
    formatCurrentDateTime("%Y-%m-%d %H:%M:%S") + " " +
    severityToString(severity) + 
      ((severity >= LoggingSeverity::Warning)
        ? " at " + getFilenameFromPath(file) + ":" + function + ":" + std::to_string(line)
        : "") +
    "] " + message;

  std::lock_guard<std::mutex> lock(m_mutex);
  for (const auto& strategy : m_loggingStrategies)
  {
    strategy->deliverMessage(severity, text);
  }
}

void LoggingManager::addLoggingStrategy(const std::shared_ptr<ILoggingStrategy>& strategy)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_loggingStrategies.emplace_back(strategy);
}

LoggingManager::LoggingManager()
  : ILoggingManager()
  , m_minSeverity(LoggingSeverity::Debug)
{
}

std::unique_ptr<ILoggingManager> ILoggingManager::m_instance;

}
