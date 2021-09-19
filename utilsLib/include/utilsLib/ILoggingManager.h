#pragma once

#include <memory>
#include <string>

#include <utilsLib/ILoggingStrategy.h>
#include <utilsLib/LoggingSeverity.h>

namespace utilsLib
{

class ILoggingStrategy;

class ILoggingManager
{
public:
  virtual ~ILoggingManager() = default;

  ILoggingManager(const ILoggingManager&) = delete;
  ILoggingManager(ILoggingManager&&) = delete;
  ILoggingManager& operator=(const ILoggingManager&) = delete;

  template <typename T,
    typename = typename std::enable_if<std::is_base_of<ILoggingManager, T>::value>::type>
  static void create()
  {
    m_instance = std::unique_ptr<T>(new T());
  }

  static void destroy()
  {
    m_instance.reset(nullptr);
  }

  static const std::unique_ptr<ILoggingManager>& getLogger()
  {
    return m_instance;
  }

  virtual void setLogLevelMinimumSeverity(LoggingSeverity severity) = 0;
  virtual void logMessage(LoggingSeverity severity, const std::string& message) = 0;
  virtual void addLoggingStrategy(const std::shared_ptr<ILoggingStrategy>& strategy) = 0;

protected:
  ILoggingManager() = default;

private:
  static std::unique_ptr<ILoggingManager> m_instance;

};

}