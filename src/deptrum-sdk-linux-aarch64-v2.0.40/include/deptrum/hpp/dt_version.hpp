// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_version.hpp
 * @brief Provides static methods to retrieve SDK version information.
 */

#ifndef DEPTRUM_HPP_DT_VERSION_HPP_
#define DEPTRUM_HPP_DT_VERSION_HPP_

#include "deptrum/h/dt_version.h"

namespace dt {

/**
 * @class Version
 * @brief Provides access to SDK version information through static methods.
 */
class Version {
 public:
  /**
   * @brief Retrieves the full SDK version number.
   * @details Full version is computed as: major*10000000+minor*10000+patch.
   * @return Full SDK version number.
   */
  static int GetVersion() { return dt_get_version(); }

  /**
   * @brief Retrieves the SDK version as a null-terminated string.
   * The format is "major.minor.patch-stage".
   * For example, "1.2.3-alpha".
   * The returned string has static storage duration and should not be freed.
   * @return const char* Null-terminated string representing the SDK version.
   */
  static const char* GetVersionStr() { return dt_get_version_str(); }

  /**
   * @brief Gets the major version component of the SDK.
   * @return Major version number.
   */
  static int GetMajor() { return dt_get_major_version(); }

  /**
   * @brief Gets the minor version component of the SDK.
   * @return Minor version number.
   */
  static int GetMinor() { return dt_get_minor_version(); }

  /**
   * @brief Gets the patch version component of the SDK.
   * @return Patch version number.
   */
  static int GetPatch() { return dt_get_patch_version(); }

  /**
   * @brief Retrieves the stage version string for special releases.
   * @return Vendor-defined stage version string. The string lifetime is managed by the SDK.
   */
  static const char* GetStageVersion() { return dt_get_stage_version(); }
};

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_VERSION_HPP_
