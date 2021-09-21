#include <gtest/gtest.h>

#include <utilsLib/Utils.h>
#include <utilsLib/ILoggingManager.h>
#include <utilsLib/LoggingManager.h>
#include <utilsLib/StdOutLoggingStrategy.h>
#include <utilsLib/FileLoggingStrategy.h>

#include <filesystem>
#include <vector>
#include <regex>
#include <fstream>

template <typename TStream>
std::vector<std::string> getStreamLines(TStream& stream)
{
  std::vector<std::string> lines;
  std::string line;    
  while (std::getline(stream, line))
  {
    lines.emplace_back(line);
  }

  return lines;
}

std::vector<std::string> getFileLines(const std::string& filename)
{
  std::ifstream stream(filename, std::ifstream::in);
  if (!stream.is_open())
  {
    return {};
  }
  return getStreamLines(stream);
}

std::vector<std::string> getStringLines(const std::string& str)
{
  std::istringstream stream(str);
  return getStreamLines(stream);
}

std::string getElementIfExists(const std::vector<std::string>& list, int index)
{
  return (index < 0 || index >= list.size()) ? "" : list.at(index);
}

std::regex buildLogMessageRegex(const std::string& severityString, const std::string& message,
                                bool containsTraceInfo)
{
  return std::regex("\\[\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2} " + severityString +
    (containsTraceInfo ? " at (\\w|\\.)+:(\\w|:)+:\\d+" : "") + "\\] " + message);
}

TEST(LoggingManagerTest, LogMessages)
{
  const std::filesystem::path logPath("./Logs");

  utilsLib::ILoggingManager::create<utilsLib::LoggingManager>();

  auto fileLoggingStrategy = std::make_shared<utilsLib::FileLoggingStrategy>(logPath.string());
  const auto logFilename   = fileLoggingStrategy->getFilename();

  utilsLib::ILoggingManager::getLogger()->addLoggingStrategy(
    std::make_shared<utilsLib::StdOutLoggingStrategy>());
  utilsLib::ILoggingManager::getLogger()->addLoggingStrategy(fileLoggingStrategy);

  utilsLib::ILoggingManager::getLogger()->setLogLevelMinimumSeverity(utilsLib::LoggingSeverity::Trace);

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  UTILS_LOG_WARN("Test");
  UTILS_LOG_ERROR("Test");
  UTILS_LOG_FATAL("Test");

  UTILS_LOG_INFO("Test");
  UTILS_LOG_TRACE("Test");
  UTILS_LOG_DEBUG("Test");

  EXPECT_TRUE(std::filesystem::exists(logFilename));

  const auto capturedStdout = getStringLines(testing::internal::GetCapturedStdout());
  const auto capturedStderr = getStringLines(testing::internal::GetCapturedStderr());
  const auto logFileLines   = getFileLines(logFilename);

#ifdef _DEBUG
  EXPECT_EQ(capturedStdout.size(), 3);
  EXPECT_EQ(logFileLines.size(), 6);
#else
  EXPECT_EQ(capturedStdout.size(), 1);
  EXPECT_EQ(logFileLines.size(), 4);
#endif
  EXPECT_EQ(capturedStderr.size(), 3);

  const auto messageWarn  = getElementIfExists(capturedStderr, 0);
  const auto messageError = getElementIfExists(capturedStderr, 1);
  const auto messageFatal = getElementIfExists(capturedStderr, 2);

  const auto fileMessageWarn  = getElementIfExists(logFileLines, 0);
  const auto fileMessageError = getElementIfExists(logFileLines, 1);
  const auto fileMessageFatal = getElementIfExists(logFileLines, 2);

  const auto messageInfo = getElementIfExists(capturedStdout, 0);

  const auto fileMessageInfo = getElementIfExists(logFileLines, 3);
#ifdef _DEBUG
  const auto messageTrace = getElementIfExists(capturedStdout, 1);
  const auto messageDebug = getElementIfExists(capturedStdout, 2);

  const auto fileMessageTrace = getElementIfExists(logFileLines, 4);
  const auto fileMessageDebug = getElementIfExists(logFileLines, 5);
#endif

  std::cmatch match;

  EXPECT_TRUE(std::regex_match(messageWarn.c_str(), match, buildLogMessageRegex("WARNING", "Test", true)));
  EXPECT_TRUE(std::regex_match(messageError.c_str(), match, buildLogMessageRegex("ERROR",   "Test", true)));
  EXPECT_TRUE(std::regex_match(messageFatal.c_str(), match, buildLogMessageRegex("FATAL",  "Test", true)));

  EXPECT_TRUE(std::regex_match(fileMessageWarn.c_str(), match, buildLogMessageRegex("WARNING", "Test", true)));
  EXPECT_TRUE(std::regex_match(fileMessageError.c_str(), match, buildLogMessageRegex("ERROR",   "Test", true)));
  EXPECT_TRUE(std::regex_match(fileMessageFatal.c_str(), match, buildLogMessageRegex("FATAL",  "Test", true)));

  EXPECT_TRUE(std::regex_match(messageInfo.c_str(), match, buildLogMessageRegex("INFO", "Test", false)));

  EXPECT_TRUE(std::regex_match(fileMessageInfo.c_str(), match, buildLogMessageRegex("INFO", "Test", false)));
#ifdef _DEBUG
  EXPECT_TRUE(std::regex_match(messageTrace.c_str(), match, buildLogMessageRegex("TRACE", "Test", false)));
  EXPECT_TRUE(std::regex_match(messageDebug.c_str(), match, buildLogMessageRegex("DEBUG", "Test", false)));

  EXPECT_TRUE(std::regex_match(fileMessageTrace.c_str(), match, buildLogMessageRegex("TRACE", "Test", false)));
  EXPECT_TRUE(std::regex_match(fileMessageDebug.c_str(), match, buildLogMessageRegex("DEBUG", "Test", false)));
#endif

  fileLoggingStrategy.reset();
  utilsLib::ILoggingManager::destroy();

  for (auto& entry : std::filesystem::directory_iterator(logPath))
  {
    if (entry.is_regular_file())
    {
      std::filesystem::remove(entry.path());
    }
  }
  std::filesystem::remove(logPath);
}
