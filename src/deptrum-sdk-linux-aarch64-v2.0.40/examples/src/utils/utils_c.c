// Copyright (c) Deptrum Inc. All Rights Reserved.

#include "utils_c.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__) || defined(__APPLE__)
#ifdef __linux__
#include <termio.h>
#else
#include <termios.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define gets_s gets

int Getch(void) {
  struct termios tm, tm_old;
  int fd = 0, ch;

  if (tcgetattr(fd, &tm) < 0) {  // Save the current terminal settings
    return -1;
  }

  tm_old = tm;
  cfmakeraw(&tm);  // Change the terminal settings to raw mode, in which all input data is processed
                   // in bytes
  if (tcsetattr(fd, TCSANOW, &tm) < 0) {  // Settings after changes on settings
    return -1;
  }

  ch = getchar();
  if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {  // Change the settings to what they were originally
    return -1;
  }

  return ch;
}

int Kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}

#include <sys/time.h>
uint64_t dt_utils_get_current_timestamp_ms() {
  struct timeval te;
  gettimeofday(&te, NULL);                                          // Get the current time
  long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;  // Calculate milliseconds
  return milliseconds;
}

char dt_utils_wait_for_key_press(uint32_t timeout_ms) {  // Get the current time
  struct timeval te;
  gettimeofday(&te, NULL);
  long long start_time = te.tv_sec * 1000LL + te.tv_usec / 1000;

  while (true) {
    if (Kbhit()) {
      return Getch();
    }
    gettimeofday(&te, NULL);
    long long current_time = te.tv_sec * 1000LL + te.tv_usec / 1000;
    if (timeout_ms > 0 && current_time - start_time > timeout_ms) {
      return 0;
    }
    usleep(100);
  }
}

#else  // Windows
#include <conio.h>
#include <windows.h>

uint64_t dt_utils_get_current_timestamp_ms() {
  FILETIME ft;
  LARGE_INTEGER li;
  GetSystemTimeAsFileTime(&ft);
  li.LowPart = ft.dwLowDateTime;
  li.HighPart = ft.dwHighDateTime;
  long long milliseconds = li.QuadPart / 10000LL;
  return milliseconds;
}

char dt_utils_wait_for_key_press(uint32_t timeout_ms) {
  HANDLE h_stdin = GetStdHandle(STD_INPUT_HANDLE);
  if (h_stdin == INVALID_HANDLE_VALUE) {
    return 0;
  }
  DWORD mode = 0;
  if (!GetConsoleMode(h_stdin, &mode)) {
    return 0;
  }
  mode &= ~ENABLE_ECHO_INPUT;
  if (!SetConsoleMode(h_stdin, mode)) {
    return 0;
  }
  DWORD start_time = GetTickCount();
  while (true) {
    if (_kbhit()) {
      char ch = (char) _getch();
      SetConsoleMode(h_stdin, mode);
      return ch;
    }
    if (timeout_ms > 0 && GetTickCount() - start_time > timeout_ms) {
      SetConsoleMode(h_stdin, mode);
      return 0;
    }
    Sleep(1);
  }
}
#endif

#ifdef __cplusplus
}
#endif
