#include <gtest/gtest.h>

#include <utilsLib/ILoggingManager.h>
#include <utilsLib/LoggingManager.h>
#include <utilsLib/StdOutLoggingStrategy.h>

TEST(LoggingManagerTest, LogMessages)
{
  utilsLib::ILoggingManager::create<utilsLib::LoggingManager>();

  utilsLib::ILoggingManager::getLogger()->addLoggingStrategy(std::make_shared<utilsLib::StdOutLoggingStrategy>());
  utilsLib::ILoggingManager::getLogger()->logMessage(utilsLib::LoggingSeverity::Info, "Test");
}
