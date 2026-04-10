// # Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <functional>
#include <iostream>
#include "utils.hpp"

int main() try {
  // Configure the log level output to the terminal.
  dt::Logger::SetLoggerToConsole(kDtLogSeverityError);

  // Configure the log level and path output to the file.
  dt::Logger::SetLoggerToFile(kDtLogSeverityDebug, "Log/Custom/");

  // Register a log callback, you can get log information in the callback.
  dt::Logger::SetLoggerToCallback(kDtLogSeverityDebug,
                                  [](DtLogSeverity severity, const char* log_msg) {
                                    std::cout << "[CallbackMessage][Level:" << severity << "]"
                                              << log_msg;
                                  });

  // Configure which streams to enable or disable for the Pipeline by creating a Config
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();
  config->EnableVideoStream(kDtStreamDepth, kDtWidthAny, kDtHeightAny, kDtFpsAny, kDtFormatY16);
  config->EnableVideoStream(kDtStreamColor, kDtWidthAny, kDtHeightAny, kDtFpsAny, kDtFormatRgb);

  // Create a pipeline with default device to manage stream
  std::shared_ptr<dt::Pipeline> pipe = std::make_shared<dt::Pipeline>();

  // Start the pipeline with config
  pipe->Start(config);
  // Stop the Pipeline, no frame data will be generated
  pipe->Stop();

  dt::Logger::SetLoggerToCallback(kDtLogSeverityOff, nullptr);
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
