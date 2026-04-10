// Copyright (c) Deptrum Inc. All Rights Reserved.
#pragma once
#if _WIN32
#include <Windows.h>
#endif

#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cmath>
#include <deptrum/hpp/dt_utils.hpp>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include "deptrum/deptrum.hpp"
#include "stb_easy_font.h"
#include "utils.hpp"
#include "utils_types.h"

namespace dt_utils {

#ifndef PI
#define PI 3.14159265358979323846
#define PI_FL 3.141592f
#endif
const float IMU_FRAME_WIDTH = 1280.f;
const float IMU_FRAME_HEIGHT = 720.f;
enum class Priority { high = 0, medium = -1, low = -2 };

//////////////////////////////
// Basic Data Types         //
//////////////////////////////

struct float3 {
  float x, y, z;
  float3 operator*(float t) { return {x * t, y * t, z * t}; }

  float3 operator-(float t) { return {x - t, y - t, z - t}; }

  void operator*=(float t) {
    x = x * t;
    y = y * t;
    z = z * t;
  }

  void operator=(float3 other) {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  void add(float t1, float t2, float t3) {
    x += t1;
    y += t2;
    z += t3;
  }
};
struct float2 {
  float x, y;
};
struct frame_pixel {
  int frame_idx;
  float2 pixel;
};

struct rect {
  float x, y;
  float w, h;

  // Create new rect within original boundaries with give aspect ration
  rect adjust_ratio(float2 size) const {
    auto H = static_cast<float>(h), W = static_cast<float>(h) * size.x / size.y;
    if (W > w) {
      auto scale = w / W;
      W *= scale;
      H *= scale;
    }

    return {x + (w - W) / 2, y + (h - H) / 2, W, H};
  }
};

struct tile_properties {
  unsigned int x, y;    // location of tile in the grid
  unsigned int w, h;    // width and height by number of tiles
  DtOptionId priority;  // when should the tile be drawn?: high priority is on top of all,
                        // medium is a layer under top layer, low is a layer under medium layer
};

// name aliasing the map of pairs<frame, tile_properties>
using frame_and_tile_property = std::pair<std::shared_ptr<dt::Frame>, tile_properties>;
using frames_mosaic = std::map<int, frame_and_tile_property>;

//////////////////////////////
// Simple font loading code //
//////////////////////////////

inline void draw_text(int x, int y, const char* text, float scale = 2.5f) {
  std::vector<char> buffer;
  buffer.resize(60000);  // ~300 chars
  glPushMatrix();
  glTranslatef((float) x, (float) y, 0.0f);
  glScalef(scale, scale, 1.0f);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 16, buffer.data());

  int num_quads = stb_easy_font_print(0.0f,
                                      0.0f,
                                      (char*) text,
                                      nullptr,
                                      buffer.data(),
                                      (int) buffer.size());
  glDrawArrays(GL_QUADS, 0, 4 * num_quads);

  glDisableClientState(GL_VERTEX_ARRAY);
  glPopMatrix();
}

void set_viewport(const rect& r) {
  static rect last_viewport{0, 0, 0, 0};
  if (last_viewport.x != r.x || last_viewport.y != r.y || last_viewport.w != r.w ||
      last_viewport.h != r.h) {
    glViewport((int) r.x, (int) r.y, (int) r.w, (int) r.h);
    last_viewport = r;
  }
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glOrtho(0, r.w, r.h, 0, -1, +1);
}

class imu_renderer {
 public:
  void render(const std::shared_ptr<dt::Frame>& frame, const rect& r) {
    draw_motion(frame, r.adjust_ratio({IMU_FRAME_WIDTH, IMU_FRAME_HEIGHT}));
  }

  GLuint get_gl_handle() { return _gl_handle; }

 private:
  GLuint _gl_handle = 0;

  void draw_motion(const std::shared_ptr<dt::Frame>& f, const rect& r) {
    if (!_gl_handle)
      glGenTextures(1, &_gl_handle);

    set_viewport(r);
    std::stringstream ss;
    ss << f->GetStreamProfile()->GetType();
    draw_text(int(0.05f * r.w), int(0.05f * r.h), ss.str().c_str());

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if (f->GetType() == DtFrameType::kDtFrameAccel) {
      auto accel = f->As<dt::AccelFrame>();
      x = accel->GetValue().x;
      y = accel->GetValue().y;
      z = accel->GetValue().z;
    } else if (f->GetType() == DtFrameType::kDtFrameGyro) {
      auto gyro = f->As<dt::GyroFrame>();
      x = gyro->GetValue().x;
      y = gyro->GetValue().y;
      z = gyro->GetValue().z;
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(-2.8, 2.8, -2.4, 2.4, -7, 7);

    glRotatef(25, 1.0f, 0.0f, 0.0f);

    glTranslatef(0, -0.33f, -1.f);

    glRotatef(-135, 0.0f, 1.0f, 0.0f);

    glRotatef(180, 0.0f, 0.0f, 1.0f);
    glRotatef(-90, 0.0f, 1.0f, 0.0f);

    draw_axes(1, 2);

    draw_circle(1, 0, 0, 0, 1, 0);
    draw_circle(0, 1, 0, 0, 0, 1);
    draw_circle(1, 0, 0, 0, 0, 1);

    const auto canvas_size = 230;
    const auto vec_threshold = 0.01f;
    float norm = std::sqrt(x * x + y * y + z * z);
    if (norm < vec_threshold) {
      const auto radius = 0.05;
      static const int circle_points = 100;
      static const float angle = 2.0f * 3.1416f / circle_points;

      glColor3f(1.0f, 1.0f, 1.0f);
      glBegin(GL_POLYGON);
      double angle1 = 0.0;
      glVertex2d(radius * cos(0.0), radius * sin(0.0));
      int i;
      for (i = 0; i < circle_points; i++) {
        glVertex2d(radius * cos(angle1), radius * sin(angle1));
        angle1 += angle;
      }
      glEnd();
    } else {
      auto vectorWidth = 3.f;
      glLineWidth(vectorWidth);
      glBegin(GL_LINES);
      glColor3f(1.0f, 1.0f, 1.0f);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(x / norm, y / norm, z / norm);
      glEnd();

      // Save model and projection matrix for later
      GLfloat model[16];
      glGetFloatv(GL_MODELVIEW_MATRIX, model);
      GLfloat proj[16];
      glGetFloatv(GL_PROJECTION_MATRIX, proj);

      glLoadIdentity();
      glOrtho(-canvas_size, canvas_size, -canvas_size, canvas_size, -1, +1);

      std::ostringstream s1;
      const auto precision = 3;

      glRotatef(180, 1.0f, 0.0f, 0.0f);

      s1 << "(" << std::fixed << std::setprecision(precision) << x << "," << std::fixed
         << std::setprecision(precision) << y << "," << std::fixed << std::setprecision(precision)
         << z << ")";
      print_text_in_3d(x, y, z, s1.str().c_str(), false, model, proj, 1 / norm);

      std::ostringstream s2;
      s2 << std::setprecision(precision) << norm;
      print_text_in_3d(x / 2, y / 2, z / 2, s2.str().c_str(), true, model, proj, 1 / norm);
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
  }

  // IMU drawing helper functions
  void multiply_vector_by_matrix(GLfloat vec[], GLfloat mat[], GLfloat* result) {
    const auto N = 4;
    for (int i = 0; i < N; i++) {
      result[i] = 0;
      for (int j = 0; j < N; j++) {
        result[i] += vec[j] * mat[N * j + i];
      }
    }
    return;
  }

  float2 xyz_to_xy(float x, float y, float z, GLfloat model[], GLfloat proj[], float vec_norm) {
    GLfloat vec[4] = {x, y, z, 0};
    float tmp_result[4];
    float result[4];

    const auto canvas_size = 230;

    multiply_vector_by_matrix(vec, model, tmp_result);
    multiply_vector_by_matrix(tmp_result, proj, result);

    return {canvas_size * vec_norm * result[0], canvas_size * vec_norm * result[1]};
  }

  void print_text_in_3d(float x,
                        float y,
                        float z,
                        const char* text,
                        bool center_text,
                        GLfloat model[],
                        GLfloat proj[],
                        float vec_norm) {
    auto xy = xyz_to_xy(x, y, z, model, proj, vec_norm);
    auto w = (center_text) ? stb_easy_font_width((char*) text) : 0;
    glColor3f(1.0f, 1.0f, 1.0f);
    draw_text((int) (xy.x - w / 2), (int) xy.y, text);
  }

  static void draw_axes(float axis_size = 1.f, float axisWidth = 4.f) {
    // Triangles For X axis
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(axis_size * 1.1f, 0.f, 0.f);
    glVertex3f(axis_size, -axis_size * 0.05f, 0.f);
    glVertex3f(axis_size, axis_size * 0.05f, 0.f);
    glVertex3f(axis_size * 1.1f, 0.f, 0.f);
    glVertex3f(axis_size, 0.f, -axis_size * 0.05f);
    glVertex3f(axis_size, 0.f, axis_size * 0.05f);
    glEnd();

    // Triangles For Y axis
    glBegin(GL_TRIANGLES);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.f, axis_size * 1.1f, 0.0f);
    glVertex3f(0.f, axis_size, 0.05f * axis_size);
    glVertex3f(0.f, axis_size, -0.05f * axis_size);
    glVertex3f(0.f, axis_size * 1.1f, 0.0f);
    glVertex3f(0.05f * axis_size, axis_size, 0.f);
    glVertex3f(-0.05f * axis_size, axis_size, 0.f);
    glEnd();

    // Triangles For Z axis
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 1.1f * axis_size);
    glVertex3f(0.0f, 0.05f * axis_size, 1.0f * axis_size);
    glVertex3f(0.0f, -0.05f * axis_size, 1.0f * axis_size);
    glVertex3f(0.0f, 0.0f, 1.1f * axis_size);
    glVertex3f(0.05f * axis_size, 0.f, 1.0f * axis_size);
    glVertex3f(-0.05f * axis_size, 0.f, 1.0f * axis_size);
    glEnd();

    glLineWidth(axisWidth);

    // Drawing Axis
    glBegin(GL_LINES);
    // X axis - Red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(axis_size, 0.0f, 0.0f);

    // Y axis - Green
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, axis_size, 0.0f);

    // Z axis - Blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, axis_size);
    glEnd();
  }

  // intensity is grey intensity
  static void draw_circle(float xx,
                          float xy,
                          float xz,
                          float yx,
                          float yy,
                          float yz,
                          float radius = 1.1,
                          float3 center = {0.0, 0.0, 0.0},
                          float intensity = 0.5f) {
    const auto N = 50;
    glColor3f(intensity, intensity, intensity);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= N; i++) {
      const double theta = (2 * PI / N) * i;
      const auto cost = static_cast<float>(cos(theta));
      const auto sint = static_cast<float>(sin(theta));
      glVertex3f(center.x + radius * (xx * cost + yx * sint),
                 center.y + radius * (xy * cost + yy * sint),
                 center.z + radius * (xz * cost + yz * sint));
    }
    glEnd();
  }
};

/// \brief Print flat 2D text over openGl window
struct text_renderer {
  // Provide textual representation only
  void put_text(const std::string& msg, float norm_x_pos, float norm_y_pos, const rect& r) {
    set_viewport(r);
    draw_text(int(norm_x_pos * r.w), int(norm_y_pos * r.h), msg.c_str());
  }
};

////////////////////////
// Image display code //
////////////////////////
/// \brief The texture class
class texture {
 public:
  texture() { glGenTextures(1, &_gl_handle); }

  ~texture() {
    if (_gl_handle) {
      glDeleteTextures(1, &_gl_handle);
      _gl_handle = 0;
    }
  }

  void upload(const std::shared_ptr<dt::VideoFrame>& frame) {
    if (!frame->GetData())
      return;

    auto format = frame->GetStreamProfile()->GetFormat();
    auto width = frame->GetWidth();
    auto height = frame->GetHeight();
    _stream_type = frame->GetStreamProfile()->GetType();
    _frame_name = frame->GetName();

    glBindTexture(GL_TEXTURE_2D, _gl_handle);

    switch (format) {
      case kDtFormatRgb:
      case kDtFormatBgr:
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     width,
                     height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     frame->GetData());
        break;
      case kDtFormatRgba:
      case kDtFormatBgra:
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     width,
                     height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     frame->GetData());
        break;
      case kDtFormatY8:
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     width,
                     height,
                     0,
                     GL_LUMINANCE,
                     GL_UNSIGNED_BYTE,
                     frame->GetData());
        break;
      case kDtFormatY10:
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_LUMINANCE,
                     width,
                     height,
                     0,
                     GL_LUMINANCE,
                     GL_UNSIGNED_SHORT,
                     frame->GetData());
        break;
      case kDtFormatY16: {
        uint16_t* depth_data = reinterpret_cast<uint16_t*>(frame->GetData());
        int width = frame->GetWidth();
        int height = frame->GetHeight();

        std::vector<uint8_t> rgb_buffer(width * height * 3);

        int range_min = 0;
        int range_mid = 1000;
        int range_max = 8000;

        bool ret = dt::Visualizer::ColorizeDepth(rgb_buffer.data(),
                                                 height,
                                                 width,
                                                 range_min,
                                                 range_mid,
                                                 range_max,
                                                 depth_data);
        if (ret != true) {
          throw std::runtime_error("ColorizeDepth failed.");
        }
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     width,
                     height,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     rgb_buffer.data());
      } break;
      default:
        throw std::runtime_error("The requested format is not supported by this demo!");
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void show(const std::shared_ptr<const dt::Frame>& frame, const rect& r, float alpha = 1.f) const {
    if (!_gl_handle)
      return;

    set_viewport(r);

    glBindTexture(GL_TEXTURE_2D, _gl_handle);
    glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(0, 1);
    glVertex2f(0, r.h);
    glTexCoord2f(1, 1);
    glVertex2f(r.w, r.h);
    glTexCoord2f(1, 0);
    glVertex2f(r.w, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    int x = int(0.05f * r.w);
    int y = int(0.05f * r.h);
    float scale = 2.5f;
    int line_height = static_cast<int>(12 * scale);

    int line = 0;

    draw_text(x, y + line_height * line++, _frame_name.c_str(), scale);
    draw_text(x,
              y + line_height * line++,
              ("Timestamp: " + std::to_string(frame->GetTimeStampUs())).c_str(),
              scale);
    draw_text(x,
              y + line_height * line++,
              ("System TS: " + std::to_string(frame->GetSystemTimeStampUs())).c_str(),
              scale);

    if (frame->HasMetadata(kDtFrameMetadataTypeTxTemperature)) {
      auto value = frame->GetMetadataValue(kDtFrameMetadataTypeTxTemperature);
      draw_text(x, y + line_height * line++, ("Tx Temp: " + std::to_string(value)).c_str(), scale);
    }
    if (frame->HasMetadata(kDtFrameMetadataTypeMtofExposure)) {
      auto value = frame->GetMetadataValue(kDtFrameMetadataTypeMtofExposure);
      draw_text(x, y + line_height * line++, ("MToF Exp: " + std::to_string(value)).c_str(), scale);
    }
    if (frame->HasMetadata(kDtFrameMetadataTypeStofExposure)) {
      auto value = frame->GetMetadataValue(kDtFrameMetadataTypeStofExposure);
      draw_text(x, y + line_height * line++, ("SToF Exp: " + std::to_string(value)).c_str(), scale);
    }
    if (frame->HasMetadata(kDtFrameMetadataTypeExposure)) {
      auto value = frame->GetMetadataValue(kDtFrameMetadataTypeExposure);
      draw_text(x, y + line_height * line++, ("Exposure: " + std::to_string(value)).c_str(), scale);
    }
    if (frame->HasMetadata(kDtFrameMetadataTypeGain)) {
      auto value = frame->GetMetadataValue(kDtFrameMetadataTypeGain);
      draw_text(x, y + line_height * line++, ("Gain: " + std::to_string(value)).c_str(), scale);
    }
  }

  GLuint get_gl_handle() { return _gl_handle; }

  void render(const std::shared_ptr<dt::Frame>& frame, const rect& rect, float alpha = 1.f) {
    if (frame->Is<dt::VideoFrame>()) {
      auto vf = frame->As<dt::VideoFrame>();
      upload(vf);
      show(frame,
           rect.adjust_ratio(
               {static_cast<float>(vf->GetWidth()), static_cast<float>(vf->GetHeight())}),
           alpha);
    } else if (auto mf = frame->Is<dt::AccelFrame>()) {
      _imu_render.render(frame, rect.adjust_ratio({IMU_FRAME_WIDTH, IMU_FRAME_HEIGHT}));
    } else if (auto mf = frame->Is<dt::GyroFrame>()) {
      _imu_render.render(frame, rect.adjust_ratio({IMU_FRAME_WIDTH, IMU_FRAME_HEIGHT}));
    } else
      throw std::runtime_error(
          "Rendering is currently supported for video, motion and pose frames only");
  }

 private:
  GLuint _gl_handle = 0;
  std::string _frame_name;
  DtStreamType _stream_type = kDtStreamUnknown;
  imu_renderer _imu_render;
};

class window {
 public:
  std::function<void(bool)> on_left_mouse = [](bool) {};
  std::function<void(double, double)> on_mouse_scroll = [](double, double) {};
  std::function<void(double, double)> on_mouse_move = [](double, double) {};
  std::function<void(int)> on_key_release = [](int) {};

  window(int width, int height, const char* title) :
      _width(width),
      _height(height),
      _canvas_left_top_x(0),
      _canvas_left_top_y(0),
      _canvas_width(width),
      _canvas_height(height) {
    glfwInit();
    // Ensure double buffering is enabled
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
    win = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!win)
      throw std::runtime_error("Could not open OpenGL window, please check your graphic drivers or "
                               "use the textual SDK tools");
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(win, this);
    glfwSetMouseButtonCallback(win, [](GLFWwindow* w, int button, int action, int mods) {
      auto s = (window*) glfwGetWindowUserPointer(w);
      if (button == 0)
        s->on_left_mouse(action == GLFW_PRESS);
    });

    glfwSetScrollCallback(win, [](GLFWwindow* w, double xoffset, double yoffset) {
      auto s = (window*) glfwGetWindowUserPointer(w);
      s->on_mouse_scroll(xoffset, yoffset);
    });

    glfwSetCursorPosCallback(win, [](GLFWwindow* w, double x, double y) {
      auto s = (window*) glfwGetWindowUserPointer(w);
      s->on_mouse_move(x, y);
    });

    glfwSetKeyCallback(win, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
      auto s = (window*) glfwGetWindowUserPointer(w);
      if (0 == action)  // on key release
      {
        s->on_key_release(key);
      }
    });
  }

  // another c'tor for adjusting specific frames in specific tiles, this window is NOT resizeable
  window(unsigned width,
         unsigned height,
         const char* title,
         unsigned tiles_in_row,
         unsigned tiles_in_col,
         float canvas_width = 0.8f,
         float canvas_height = 0.6f,
         float canvas_left_top_x = 0.1f,
         float canvas_left_top_y = 0.075f) :
      _width(width),
      _height(height),
      _tiles_in_row(tiles_in_row),
      _tiles_in_col(tiles_in_col)

  {
    // user input verification for mosaic size, if invalid values were given - set to default
    if (canvas_width < 0 || canvas_width > 1 || canvas_height < 0 || canvas_height > 1 ||
        canvas_left_top_x < 0 || canvas_left_top_x > 1 || canvas_left_top_y < 0 ||
        canvas_left_top_y > 1) {
      std::cout << "Invalid window's size parameter entered, setting to default values"
                << std::endl;
      canvas_width = 0.8f;
      canvas_height = 0.6f;
      canvas_left_top_x = 0.15f;
      canvas_left_top_y = 0.075f;
    }

    // user input verification for number of tiles in row and column
    if (_tiles_in_row <= 0) {
      _tiles_in_row = 4;
    }
    if (_tiles_in_col <= 0) {
      _tiles_in_col = 2;
    }

    // calculate canvas size
    _canvas_width = int(_width * canvas_width);
    _canvas_height = int(_height * canvas_height);
    _canvas_left_top_x = _width * canvas_left_top_x;
    _canvas_left_top_y = _height * canvas_left_top_y;

    // calculate tile size
    _tile_width_pixels = float(std::floor(_canvas_width / _tiles_in_row));
    _tile_height_pixels = float(std::floor(_canvas_height / _tiles_in_col));

    glfwInit();
    // we don't want to enable resizing the window
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);  // Ensure double buffering is enabled
    win = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!win)
      throw std::runtime_error("Could not open OpenGL window, please check your graphic drivers or "
                               "use the textual SDK tools");
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(win, this);
    glfwSetMouseButtonCallback(win, [](GLFWwindow* w, int button, int action, int mods) {
      auto s = (window*) glfwGetWindowUserPointer(w);
      if (button == 0)
        s->on_left_mouse(action == GLFW_PRESS);
    });

    glfwSetScrollCallback(win, [](GLFWwindow* w, double xoffset, double yoffset) {
      auto s = (window*) glfwGetWindowUserPointer(w);
      s->on_mouse_scroll(xoffset, yoffset);
    });

    glfwSetCursorPosCallback(win, [](GLFWwindow* w, double x, double y) {
      auto s = (window*) glfwGetWindowUserPointer(w);
      s->on_mouse_move(x, y);
    });

    glfwSetKeyCallback(win, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
      auto s = (window*) glfwGetWindowUserPointer(w);
      if (0 == action)  // on key release
      {
        s->on_key_release(key);
      }
    });
  }

  ~window() {
    glfwDestroyWindow(win);
    glfwTerminate();
  }

  void close() { glfwSetWindowShouldClose(win, 1); }

  float width() const { return float(_width); }
  float height() const { return float(_height); }
  void set_new_frame_flag(bool flag) { _has_new_frame = flag; }

  operator bool() {
    if (!_has_new_frame) {
      glfwPollEvents();
      return !glfwWindowShouldClose(win);
    }

    // Double buffer exchange
    glfwSwapBuffers(win);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _has_new_frame = false;

    glViewport(0, 0, _width, _height);

    // Draw the image
    glPushMatrix();
    glfwGetWindowSize(win, &_width, &_height);
    glOrtho(0, _width, _height, 0, -1, +1);

    glfwPollEvents();
    return !glfwWindowShouldClose(win);
  }

  void show(const std::shared_ptr<dt::Frame>& frame) {
    _has_new_frame = true;
    show(frame, {0, 0, static_cast<float>(_width), static_cast<float>(_height)});
  }

  void show(const std::vector<std::shared_ptr<dt::Frame>>& frames, int device_count) {
    if (frames.size() != device_count) {
      return;
    }

    _has_new_frame = true;
    float width = this->_width / device_count;
    for (int i = 0; i < device_count; i++) {
      rect sub_rect = {0 + i * width, 0, width, static_cast<float>(_height)};
      show(frames[i], sub_rect);
    }
  }

  void show(const std::shared_ptr<dt::Frame>& frame, const rect& rect) {
    if (frame->Is<dt::FrameSet>()) {
      auto fs = frame->As<dt::FrameSet>();
      render_frameset(fs, rect);
    } else if (frame->Is<dt::VideoFrame>()) {
      auto vf = frame->As<dt::VideoFrame>();
      render_video_frame(vf, rect);
    } else if (frame->Is<dt::AccelFrame>()) {
      auto fs = frame->As<dt::AccelFrame>();
      render_motion_frame(fs, rect);
    } else if (frame->Is<dt::GyroFrame>()) {
      auto fs = frame->As<dt::GyroFrame>();
      render_motion_frame(fs, rect);
    }
  }

  void show(const std::map<int, std::shared_ptr<dt::Frame>>& frames) {
    // Render openGl mosaic of frames
    if (frames.size()) {
      int cols = int(std::ceil(std::sqrt(frames.size())));
      int rows = int(std::ceil(frames.size() / static_cast<float>(cols)));

      float view_width = float(_width / cols);
      float view_height = float(_height / rows);
      unsigned int stream_no = 0;
      for (const auto& frame : frames) {
        rect viewport_loc{view_width * (stream_no % cols),
                          view_height * (stream_no / cols),
                          view_width,
                          view_height};
        show(frame.second, viewport_loc);
        stream_no++;
      }
    } else {
      _main_win.put_text("Connect one or more Intel RealSense devices and rerun the example",
                         0.4f,
                         0.5f,
                         {0.f, 0.f, float(_width), float(_height)});
    }
  }

  // gets as argument a map of the -need to be drawn- frames with their tiles properties,
  // which indicates where and what size should the frame be drawn on the canvas
  void show(const frames_mosaic& frames) {
    // Render openGl mosaic of frames
    if (frames.size()) {
      // create vector of frames from map, and sort it by priority
      std::vector<frame_and_tile_property> vector_frames;
      // copy: map (values) -> vector
      for (const auto& frame : frames) {
        vector_frames.push_back(frame.second);
      }
      // sort in ascending order of the priority
      std::sort(vector_frames.begin(),
                vector_frames.end(),
                [](const frame_and_tile_property& frame1, const frame_and_tile_property& frame2) {
                  return frame1.second.priority < frame2.second.priority;
                });
      // create margin to the shown frame on tile
      float frame_width_size_from_tile_width = 1.0f;
      // iterate over frames in ascending priority order (so that lower priority frame is drawn
      // first, and can be over-written by higher priority frame )
      for (const auto& frame : vector_frames) {
        tile_properties attr = frame.second;
        rect viewport_loc{_tile_width_pixels * attr.x + _canvas_left_top_x,
                          _tile_height_pixels * attr.y + _canvas_left_top_y,
                          _tile_width_pixels * attr.w * frame_width_size_from_tile_width,
                          _tile_height_pixels * attr.h};
        show(frame.first, viewport_loc);
      }
    } else {
      _main_win.put_text("Connect one or more Intel RealSense devices and rerun the example",
                         0.3f,
                         0.5f,
                         {float(_canvas_left_top_x),
                          float(_canvas_left_top_y),
                          float(_canvas_width),
                          float(_canvas_height)});
    }
  }

  operator GLFWwindow*() { return win; }

 private:
  GLFWwindow* win;
  bool _has_new_frame = true;
  std::map<std::string, texture> _textures;
  std::map<int, imu_renderer> _imus;
  text_renderer _main_win;
  int _width, _height;
  float _canvas_left_top_x, _canvas_left_top_y;
  int _canvas_width, _canvas_height;
  unsigned _tiles_in_row, _tiles_in_col;
  float _tile_width_pixels, _tile_height_pixels;
  std::map<std::string, std::shared_ptr<dt::Frame>> frame_cache_;

  void render_video_frame(const std::shared_ptr<dt::VideoFrame>& f, const rect& r) {
    auto& t = _textures[f->GetName()];
    t.render(f, r);
  }

  void render_motion_frame(const std::shared_ptr<dt::Frame>& f, const rect& r) {
    auto& i = _imus[f->GetFormat()];
    i.render(f, r);
  }

  void render_frameset(const std::shared_ptr<dt::FrameSet>& frames, const rect& r) {
    for (int i = 0; i < frames->GetCount(); ++i) {
      auto f = frames->GetFrameByIndex(i);
      if (f->GetType() == DtFrameType::kDtFramePoints)
        continue;
      if (!can_render(f))
        continue;

      std::string cache_key = f->GetName();
      frame_cache_[cache_key] = f;
    }

    std::vector<std::shared_ptr<dt::Frame>> cached_frames;
    for (auto& pair : frame_cache_) {
      auto& frame = pair.second;
      if (frame && can_render(frame)) {
        cached_frames.push_back(frame);
      }
    }

    if (cached_frames.empty())
      return;

    std::sort(
        cached_frames.begin(),
        cached_frames.end(),
        [](const std::shared_ptr<const dt::Frame>& a, const std::shared_ptr<const dt::Frame>& b) {
          return a->GetStreamProfile()->GetType() < b->GetStreamProfile()->GetType();
        });

    auto image_grid = calc_grid(r, cached_frames);
    for (size_t i = 0; i < cached_frames.size(); ++i) {
      show(cached_frames[i], image_grid.at(i));
    }
  }

  bool can_render(const std::shared_ptr<dt::Frame>& f) const {
    auto format = f->GetFormat();
    switch (format) {
      case kDtFormatRgb:
      case kDtFormatBgr:
      case kDtFormatRgba:
      case kDtFormatBgra:
      case kDtFormatY8:
      case kDtFormatAccel:
      case kDtFormatGyro:
      case kDtFormatY10:
      case kDtFormatY16:
        return true;
      default:
        return false;
    }
  }

  rect calc_grid(rect r, size_t streams) {
    if (r.w <= 0 || r.h <= 0 || streams <= 0)
      throw std::runtime_error(
          "invalid window configuration request, failed to calculate window grid");
    float ratio = r.w / r.h;
    auto x = sqrt(ratio * (float) streams);
    auto y = (float) streams / x;
    auto w = round(x);
    auto h = round(y);
    if (w == 0 || h == 0)
      throw std::runtime_error(
          "invalid window configuration request, failed to calculate window grid");
    while (w * h > streams)
      h > w ? h-- : w--;
    while (w * h < streams)
      h > w ? w++ : h++;
    auto new_w = round(r.w / w);
    auto new_h = round(r.h / h);
    // column count, line count, cell width cell height
    return rect{static_cast<float>(w),
                static_cast<float>(h),
                static_cast<float>(new_w),
                static_cast<float>(new_h)};
  }

  std::vector<rect> calc_grid(rect r, std::vector<std::shared_ptr<dt::Frame>>& frames) {
    auto grid = calc_grid(r, frames.size());

    std::vector<rect> rv;
    int curr_line = -1;

    for (int i = 0; i < frames.size(); i++) {
      auto mod = i % (int) grid.x;
      float fw = IMU_FRAME_WIDTH;
      float fh = IMU_FRAME_HEIGHT;
      if (frames[i]->Is<dt::VideoFrame>()) {
        auto vf = frames[i]->As<dt::VideoFrame>();
        fw = (float) vf->GetWidth();
        fh = (float) vf->GetHeight();
      }
      float cell_x_postion = (float) (mod * grid.w);
      if (mod == 0)
        curr_line++;
      float cell_y_position = curr_line * grid.h;
      float2 margin = {grid.w * 0.02f, grid.h * 0.02f};
      auto r = rect{cell_x_postion + margin.x,
                    cell_y_position + margin.y,
                    grid.w - 2 * margin.x,
                    grid.h};
      rv.push_back(r.adjust_ratio(float2{fw, fh}));
    }

    return rv;
  }
};

// Struct to get keys pressed on window
struct window_key_listener {
  int last_key = GLFW_KEY_UNKNOWN;

  window_key_listener(window& win) {
    win.on_key_release = std::bind(&window_key_listener::on_key_release,
                                   this,
                                   std::placeholders::_1);
  }

  void on_key_release(int key) {
    last_key = key;
    if (key_pressed_callback_ != nullptr) {
      key_pressed_callback_(key);
    }
  }

  int get_key() {
    int key = last_key;
    last_key = GLFW_KEY_UNKNOWN;
    return key;
  }

  void set_key_pressed_callback(std::function<void(int)> callback) {
    key_pressed_callback_ = callback;
  }

 private:
  std::function<void(int)> key_pressed_callback_;
};

// Struct for managing rotation of pointcloud view
struct glfw_state {
  glfw_state(float yaw = 15.0, float pitch = 15.0) :
      yaw(yaw),
      pitch(pitch),
      last_x(0.0),
      last_y(0.0),
      ml(false),
      offset_x(2.f),
      offset_y(2.f),
      tex() {}
  double yaw;
  double pitch;
  double last_x;
  double last_y;
  bool ml;
  float offset_x;
  float offset_y;
  texture tex;
};

// Handles all the OpenGL calls needed to display the point cloud
void draw_pointcloud(float width,
                     float height,
                     glfw_state& app_state,
                     const std::shared_ptr<dt::PointsFrame>& point_cloud_frame) {
  // Reset the open gl state
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Set the background color to light gray
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f, width / height, 0.01f, 100.0f);

  // Set up the model view matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  // Adjust the camera position to make sure the point cloud is visible
  gluLookAt(0,
            0,
            -2.0f,  // Move the camera back to ensure that the point cloud is in the field of view
            0,
            0,
            1,  // Look at the positive front direction
            0,
            -1,
            0);  // Set the up direction to negative y-axis

  // Apply user-controlled rotation and panning
  glTranslatef(0.0f, 0.0f, app_state.offset_y * 0.05f);
  glRotatef(app_state.pitch, 1.0f, 0.0f, 0.0f);
  glRotatef(app_state.yaw, 0.0f, 1.0f, 0.0f);
  glTranslatef(0, 0, -0.5f);

  // Set the dot size, making sure to be at least 1 pixel
  float point_size = width / 640.0f > 1.0f ? width / 640.0f : 1.0f;
  glPointSize(point_size);

  // Disable textures (unless you need to use texture shading)
  glDisable(GL_TEXTURE_2D);

  // Enable depth testing and spot smoothing
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  // Start drawing points
  glBegin(GL_POINTS);

  auto point_cloud_type = point_cloud_frame->GetFormat();
  auto data = point_cloud_frame->GetData();
  auto ratio = point_cloud_frame->GetCoordinateValueScale() / 1000.0f;
  size_t point_count = 0;

  if (point_cloud_type == kDtFormatPoint) {
    DtPoint* points = reinterpret_cast<DtPoint*>(const_cast<uint8_t*>(data));
    point_count = point_cloud_frame->GetDataSize() / sizeof(DtPoint);

    // Set default color (white) for colorless dots
    glColor3f(1.0f, 1.0f, 1.0f);

    for (size_t i = 0; i < point_count; i++) {
      if (fabs(points[i].z) > 0.001f) {  // Check the effective depth
        glVertex3f(points[i].x * ratio, points[i].y * ratio, points[i].z * ratio);
      }
    }
  } else if (point_cloud_type == kDtFormatRgbPoint) {
    DtColorPoint* points = reinterpret_cast<DtColorPoint*>(const_cast<uint8_t*>(data));
    point_count = point_cloud_frame->GetDataSize() / sizeof(DtColorPoint);

    for (size_t i = 0; i < point_count; i++) {
      if (fabs(points[i].z) > 0.001f) {  // Check the effective depth
        // Set the color of each point (assuming RGB values in the range 0-255)
        glColor3f(points[i].r / 255.0f, points[i].g / 255.0f, points[i].b / 255.0f);
        glVertex3f(points[i].x * ratio, points[i].y * ratio, points[i].z * ratio);
      }
    }
  }

  glEnd();

  // Reset the OpenGL state
  glDisable(GL_POINT_SMOOTH);
  glDisable(GL_DEPTH_TEST);
}

// Registers the state variable and callbacks to allow mouse control of the pointcloud
void register_glfw_callbacks(window& app, glfw_state& app_state) {
  app.on_left_mouse = [&](bool pressed) { app_state.ml = pressed; };

  app.on_mouse_scroll = [&](double xoffset, double yoffset) {
    app_state.offset_x -= static_cast<float>(xoffset);
    app_state.offset_y -= static_cast<float>(yoffset);
  };

  app.on_mouse_move = [&](double x, double y) {
    if (app_state.ml) {
      app_state.yaw -= (x - app_state.last_x);
      app_state.yaw = (app_state.yaw > -120.0 ? app_state.yaw : -120.0);
      app_state.yaw = (app_state.yaw < 120.0 ? app_state.yaw : 120.0);
      app_state.pitch += (y - app_state.last_y);
      app_state.pitch = (app_state.pitch > -80.0 ? app_state.pitch : -80.0);
      app_state.pitch = (app_state.pitch < 80.0 ? app_state.pitch : 80.0);
    }
    app_state.last_x = x;
    app_state.last_y = y;
  };

  app.on_key_release = [&](int key) {
    if (key == 32)  // Escape
    {
      app_state.yaw = app_state.pitch = 0;
      app_state.offset_x = app_state.offset_y = 0.0;
    }
  };
}

void get_screen_resolution(unsigned int& window_width, unsigned int& window_height) {
  glfwInit();
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  window_width = mode->width;
  window_height = mode->height;
}

}  // namespace dt_utils