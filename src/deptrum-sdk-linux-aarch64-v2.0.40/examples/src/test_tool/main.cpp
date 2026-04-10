#include <chrono>
#include <ctime>
#include <iostream>
#include "test_tool_device_manage.hpp"

using namespace std;
using namespace dt;
static std::shared_ptr<TestToolDeviceManage> test_tool_device_manage_ptr;

int main() {
  test_tool_device_manage_ptr = std::make_shared<TestToolDeviceManage>();
  test_tool_device_manage_ptr->Init();
  return 0;
}
