#include "test_utils.hpp"
#include <fstream>
#include <iomanip>

void SetColorBoldYellowText(const std::string text) {
#ifdef __linux__
  std::cout << BOLDYELLOW << text << std::endl;
#elif defined _WIN32
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                          FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
  std::cout << "-------------------------------" << text.c_str()
            << "-------------------------------" << std::endl;
  SetConsoleTextAttribute(
      GetStdHandle(STD_OUTPUT_HANDLE),
      FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

void SetBlueText(const std::string text) {
#ifdef __linux__
  std::cout << BLUE << text << std::endl;
#elif defined _WIN32
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                          FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
  std::cout << text.c_str() << std::endl;
  SetConsoleTextAttribute(
      GetStdHandle(STD_OUTPUT_HANDLE),
      FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

// int ReadBinFile(const std::string& filename, OpaqueData<uint8_t>& file_data) {
//   std::ifstream file(filename, std::ios::binary | std::ios::ate);
//   if (!file) {
//     LOG_F(ERROR, "Unable to open file: %s", filename.c_str());
//     return CODE_FILE_NOT_EXIST;
//   }
//   std::streamsize size = file.tellg();
//   if (size <= 0) {
//     LOG_F(ERROR, "File size invalid or empty: %s", filename.c_str());
//     return CODE_FAILED_TO_OPERATE_FILE;
//   }
//   constexpr std::streamsize MAX_FILE_SIZE = 10 * 1024 * 1024;
//   if (size > MAX_FILE_SIZE) {
//     LOG_F(ERROR, "File too large: %s", filename.c_str());
//     return CODE_FILE_IS_TOO_LARGE;
//   }

//   file.seekg(0, std::ios::beg);
//   std::shared_ptr<uint8_t> buffer(new uint8_t[size], std::default_delete<uint8_t[]>());
//   if (!file.read(reinterpret_cast<char*>(buffer.get()), size)) {
//     LOG_F(ERROR, "Failed to read file: %s", filename.c_str());
//     file_data.data.reset();
//     file_data.data_len = 0;
//     return CODE_FAILED_TO_OPERATE_FILE;
//   }

//   file_data.data = buffer;
//   file_data.data_len = static_cast<int32_t>(size);
//   return 0;
// }

// // 为 Intrinsic 重载 << 运算符
// std::ostream& operator<<(std::ostream& os, const Intrinsic& intrinsic) {
//   os << "Intrinsic: " << std::endl
//      << "  Rows: " << intrinsic.rows << std::endl
//      << "  Cols: " << intrinsic.cols << std::endl
//      << "  Focal Length: [" << intrinsic.focal_length[0] << ", " << intrinsic.focal_length[1] <<
//      "]"
//      << std::endl
//      << "  Principal Point: [" << intrinsic.principal_point[0] << ", "
//      << intrinsic.principal_point[1] << "]" << std::endl
//      << "  Distortion Coeffs: [";
//   for (int i = 0; i < 14; ++i) {
//     os << intrinsic.distortion_coeffs[i] << (i < 13 ? ", " : "]");
//   }
//   os << std::endl << "  Distortion Coeff Type: " << intrinsic.distortion_coeff_type << std::endl;
//   return os;
// }

// // 为 Extrinsic 重载 << 运算符
// std::ostream& operator<<(std::ostream& os, const Extrinsic& extrinsic) {
//   os << "Extrinsic: " << std::endl << "  Rotation Matrix: [" << std::endl;
//   for (int i = 0; i < 3; ++i) {
//     os << "    [";
//     for (int j = 0; j < 3; ++j) {
//       os << std::setw(8) << extrinsic.rotation_matrix[i * 3 + j] << (j < 2 ? ", " : "");
//     }
//     os << "]" << std::endl;
//   }
//   os << "  ]" << std::endl
//      << "  Translation Vector: [" << extrinsic.translation_vector[0] << ", "
//      << extrinsic.translation_vector[1] << ", " << extrinsic.translation_vector[2] << "]"
//      << std::endl;
//   return os;
// }

// std::ostream& operator<<(std::ostream& os, const CameraParam& camera_param) {
//   os << "Camera Parameters: " << std::endl
//      << "IR Intrinsic: " << std::endl
//      << camera_param.ir_intrinsic << "RGB Intrinsic: " << std::endl
//      << camera_param.rgb_intrinsic << "Extrinsic: " << std::endl
//      << camera_param.extrinsic;
//   return os;
// }
