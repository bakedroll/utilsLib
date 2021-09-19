#pragma once

#include <utilsLib/ILoggingStrategy.h>

#include <string>
#include <fstream>

namespace utilsLib
{

class FileLoggingStrategy : public utilsLib::ILoggingStrategy
{
public:
  FileLoggingStrategy(const std::string& directory);
  ~FileLoggingStrategy() override;

  void deliverMessage(LoggingSeverity severity, const std::string& message) override;

private:
  std::ofstream m_stream;

};

}
