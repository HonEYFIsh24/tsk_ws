// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.h>
#include <thread>
#include "utils.hpp"
#include "utils_glfw.hpp"

bool quit_program = false;  // Flag to signal the program to quit

void PrintFiltersInfo(const std::vector<std::shared_ptr<dt::Filter>>& filter_list) {
  std::cout << filter_list.size() << " post processing filters recommended:" << std::endl;
  for (auto& filter : filter_list) {
    std::cout << " - " << filter->GetName() << ": "
              << (filter->IsEnabled() ? "enabled" : "disabled") << std::endl;
    auto config_schema_vec = filter->GetConfigSchemaVec();

    // Print the config schema for each filter
    for (auto& config_schema : config_schema_vec) {
      std::cout << "    - {" << config_schema.name << ", " << config_schema.type << ", "
                << config_schema.min << ", " << config_schema.max << ", " << config_schema.step
                << ", " << config_schema.def << ", " << config_schema.desc << "}" << std::endl;
    }
    filter->Enable(false);  // Disable the filter
  }
}

void FilterControl(const std::vector<std::shared_ptr<dt::Filter>>& filter_list) {
  auto print_help = [&]() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "- Enter `[Filter]` to list the config values for the filter" << std::endl;
    std::cout << "- Enter `[Filter] on` or `[Filter] off` to enable/disable the filter"
              << std::endl;
    std::cout << "- Enter `[Filter] list` to list the config schema for the filter" << std::endl;
    std::cout << "- Enter `[Filter] [Config]` to show the config values for the filter"
              << std::endl;
    std::cout << "- Enter `[Filter] [Config] [Value]` to set a config value" << std::endl;
    std::cout << "- Enter `L`or `l` to list all available filters" << std::endl;
    std::cout << "- Enter `H` or `h` to print this help message" << std::endl;
    std::cout << "- Enter `Q` or `q` to quit" << std::endl;
  };
  print_help();
  while (!quit_program) {
    std::cout << "---------------------------" << std::endl;
    std::cout << "Enter your input (h for help): ";

    std::string input;
    std::getline(std::cin, input);
    if (input == "q" || input == "Q") {
      quit_program = true;
      break;
    } else if (input == "l" || input == "L") {
      PrintFiltersInfo(filter_list);
      continue;
    } else if (input == "h" || input == "H") {
      print_help();
      continue;
    }

    // Parse the input
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    for (std::string token; iss >> token;) {
      tokens.push_back(token);
    }
    if (tokens.empty()) {
      continue;
    }

    bool found_filter = false;
    for (auto& filter : filter_list) {
      if (filter->GetName() == tokens[0]) {
        found_filter = true;
        if (tokens.size() == 1) {  // print list of configs for the filter
          auto config_schema_vec = filter->GetConfigSchemaVec();
          std::cout << "Config values for " << filter->GetName() << ":" << std::endl;
          for (auto& config_schema : config_schema_vec) {
            std::cout << " - " << config_schema.name << ": "
                      << filter->GetConfigValue(config_schema.name) << std::endl;
          }
        } else if (tokens.size() == 2 &&
                   (tokens[1] == "on" || tokens[1] == "off")) {  // Enable/disable the filter
          filter->Enable(tokens[1] == "on");
          std::cout << "Success: Filter " << filter->GetName() << " is now "
                    << (filter->IsEnabled() ? "enabled" : "disabled") << std::endl;
        } else if (tokens.size() == 2 &&
                   tokens[1] == "list") {  // List the config values for the filter
          auto config_schema_vec = filter->GetConfigSchemaVec();
          std::cout << "Config schema for " << filter->GetName() << ":" << std::endl;
          for (auto& config_schema : config_schema_vec) {
            std::cout << " - {" << config_schema.name << ", " << config_schema.type << ", "
                      << config_schema.min << ", " << config_schema.max << ", "
                      << config_schema.step << ", " << config_schema.def << ", "
                      << config_schema.desc << "}" << std::endl;
          }
        } else if (tokens.size() == 2) {  // Print the config schema for the filter
          auto config_schema_vec = filter->GetConfigSchemaVec();
          bool found_config = false;
          for (auto& config_schema : config_schema_vec) {
            if (config_schema.name == tokens[1]) {
              found_config = true;
              std::cout << "Config values for " << filter->GetName() << "@" << config_schema.name
                        << ":" << filter->GetConfigValue(config_schema.name) << std::endl;
              break;
            }
          }
          if (!found_config) {
            std::cerr << "Error: Config " << tokens[1] << " not found for filter "
                      << filter->GetName() << std::endl;
          }
        } else if (tokens.size() == 3) {  // Set a config value
          try {
            double value = std::stod(tokens[2]);
            filter->SetConfigValue(tokens[1], value);
          } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            continue;
          }
          std::cout << "Success: Config value of " << tokens[1] << " for filter "
                    << filter->GetName() << " is set to " << tokens[2] << std::endl;
        }
        break;
      }
    }
    if (!found_filter) {
      std::cerr << "Error: Filter " << tokens[0] << " not found" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}

int main() try {
  // Create a pipeline with default device
  dt::Pipeline pipe;

  // Get the device and sensor, and get the list of recommended filters for the sensor
  auto device = pipe.GetDevice();
  auto sensor = device->GetSensor(kDtSensorDepth);
  auto filter_list = sensor->CreateRecommendedFilters();

  // Print the recommended filters
  PrintFiltersInfo(filter_list);

  // Create a config with depth stream enabled
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();
  config->EnableStream(kDtStreamDepth);

  // Start the pipeline with config
  pipe.Start(config);

  // Start the filter control loop on sub thread
  std::thread filter_control_thread(FilterControl, filter_list);
  filter_control_thread.detach();

  // Create a window for rendering, and set the resolution of the window
  dt_utils::window app(1280, 720, "PostProcessing");

  while (app) {
    // Wait for up to 1000ms for a frameset in blocking mode.
    auto frame_set = pipe.WaitForFrames(1000);
    if (frame_set == nullptr) {
      continue;
    }

    // Get the depth frame from the frameset
    auto depth_frame = frame_set->GetFrame(kDtFrameDepth);
    if (!depth_frame) {
      continue;
    }

    auto processed_frame = depth_frame;
    // Apply the recommended filters to the depth frame
    for (auto& filter : filter_list) {
      if (filter->IsEnabled()) {  // Only apply enabled filters
        processed_frame = filter->Process(processed_frame);
      }
    }

    // Push the frames to the window for showing
    // Due to processed_frame type is same as the depth_frame, we should push it with different
    // group id.
    std::vector<std::shared_ptr<dt::Frame>> frames;
    frames.push_back(depth_frame);
    frames.push_back(processed_frame);
    app.show(frames, 2);
  }

  // Stop the pipeline
  pipe.Stop();

  quit_program = true;

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
