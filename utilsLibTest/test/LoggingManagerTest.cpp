#include <gtest/gtest.h>

#include <utilsLib/Utils.h>
#include <utilsLib/ILoggingManager.h>
#include <utilsLib/LoggingManager.h>
#include <utilsLib/StdOutLoggingStrategy.h>
#include <utilsLib/FileLoggingStrategy.h>

#include <filesystem>
#include <vector>
#include <regex>

std::vector<std::string> getStringLines(const std::string& str)
{
  std::vector<std::string> lines;

  std::istringstream stream(str);
  std::string line;    
  while (std::getline(stream, line))
  {
    lines.emplace_back(line);
  }

  return lines;
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

  utilsLib::ILoggingManager::getLogger()->addLoggingStrategy(
    std::make_shared<utilsLib::StdOutLoggingStrategy>());
  //utilsLib::ILoggingManager::getLogger()->addLoggingStrategy(
  //  std::make_shared<utilsLib::FileLoggingStrategy>(logPath.string()));

  utilsLib::ILoggingManager::getLogger()->setLogLevelMinimumSeverity(utilsLib::LoggingSeverity::Trace);

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  UTILS_LOG_INFO("Test");
  UTILS_LOG_TRACE("Test");
  UTILS_LOG_DEBUG("Test");

  UTILS_LOG_WARN("Test");
  UTILS_LOG_ERROR("Test");
  UTILS_LOG_FATAL("Test");

  const auto capturedStdout = getStringLines(testing::internal::GetCapturedStdout());
  const auto capturedStderr = getStringLines(testing::internal::GetCapturedStderr());

#ifdef _DEBUG
  EXPECT_EQ(capturedStdout.size(), 3);
#else
  EXPECT_EQ(capturedStdout.size(), 1);
#endif
  EXPECT_EQ(capturedStderr.size(), 3);

  const auto messageInfo = getElementIfExists(capturedStdout, 0);
#ifdef _DEBUG
  const auto messageTrace = getElementIfExists(capturedStdout, 1);
  const auto messageDebug = getElementIfExists(capturedStdout, 2);
#endif

  const auto messageWarn  = getElementIfExists(capturedStderr, 0);
  const auto messageError = getElementIfExists(capturedStderr, 1);
  const auto messageFatal = getElementIfExists(capturedStderr, 2);

  std::cmatch match;

  EXPECT_TRUE(std::regex_match(messageInfo.c_str(), match, buildLogMessageRegex("INFO", "Test", false)));
#ifdef _DEBUG
  EXPECT_TRUE(std::regex_match(messageTrace.c_str(), match, buildLogMessageRegex("TRACE", "Test", false)));
  EXPECT_TRUE(std::regex_match(messageDebug.c_str(), match, buildLogMessageRegex("DEBUG", "Test", false)));
#endif

  EXPECT_TRUE(std::regex_match(messageWarn.c_str(), match, buildLogMessageRegex("WARNING", "Test", true)));
  EXPECT_TRUE(std::regex_match(messageError.c_str(), match, buildLogMessageRegex("ERROR",   "Test", true)));
  EXPECT_TRUE(std::regex_match(messageFatal.c_str(), match, buildLogMessageRegex("FATAL",  "Test", true)));

  utilsLib::ILoggingManager::destroy();

  if (std::filesystem::exists(logPath))
  {
    for (auto& entry : std::filesystem::directory_iterator(logPath))
    {
      if (entry.is_regular_file())
      {
        std::filesystem::remove(entry.path());
      }
    }
    std::filesystem::remove(logPath);
  }
}
