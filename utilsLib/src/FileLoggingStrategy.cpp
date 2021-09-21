#include "utilsLib/FileLoggingStrategy.h"
#include "utilsLib/Utils.h"

#include <filesystem>
#include <chrono>

namespace utilsLib
{

FileLoggingStrategy::FileLoggingStrategy(const std::string& directory)
  : ILoggingStrategy()
{
  std::filesystem::path path(directory);
  std::filesystem::create_directories(path);

  const auto filename = formatTimePoint(std::chrono::system_clock::now(), "%Y-%m-%d_%H.%M.%S") + ".txt";
  path /= filename;

  m_filename = path.string();

  m_stream.open(m_filename, std::ofstream::out);
}

FileLoggingStrategy::~FileLoggingStrategy()
{
  if (m_stream.is_open())
  {
    m_stream.close();
  }
}

std::string FileLoggingStrategy::getFilename() const
{
  return m_filename;
}

void FileLoggingStrategy::deliverMessage(LoggingSeverity severity, const std::string& message)
{
  if (m_stream.is_open())
  {
    m_stream << message << std::endl;
    m_stream.flush();
  }
}

}
