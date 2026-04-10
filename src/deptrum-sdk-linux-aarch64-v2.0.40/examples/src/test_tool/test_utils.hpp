#ifndef SRC_TEST_UTILS_H_
#define SRC_TEST_UTILS_H_

#ifdef __linux__
// the following are UBUNTU/LINUX ONLY terminal color codes.
#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#elif defined _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <string>
// #include "header.h"
void SetColorBoldYellowText(const std::string text);

void SetBlueText(const std::string text);

// int ReadBinFile(const std::string& filename, OpaqueData<uint8_t>& file_data);
// // 为 Intrinsic 重载 << 运算符
// std::ostream& operator<<(std::ostream& os, const Intrinsic& intrinsic);

// // 为 Extrinsic 重载 << 运算符
// std::ostream& operator<<(std::ostream& os, const Extrinsic& extrinsic);

// std::ostream& operator<<(std::ostream& os, const CameraParam& camera_param);

#endif  //
