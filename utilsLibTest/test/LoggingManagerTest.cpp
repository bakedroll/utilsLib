#include <gtest/gtest.h>

#include <utilsLib/Utils.h>
#include <utilsLib/ILoggingManager.h>
#include <utilsLib/LoggingManager.h>
#include <utilsLib/StdOutLoggingStrategy.h>

TEST(LoggingManagerTest, LogMessages)
{
  utilsLib::ILoggingManager::create<utilsLib::LoggingManager>();

  utilsLib::ILoggingManager::getLogger()->addLoggingStrategy(std::make_shared<utilsLib::StdOutLoggingStrategy>());

  UTILS_LOG_DEBUG("Test");
  UTILS_LOG_INFO("Test");
  UTILS_LOG_WARN("Test");
  UTILS_LOG_ERROR("Test");
}
