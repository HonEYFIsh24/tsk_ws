// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_version.h
 * @brief SDK version information retrieval interface
 */

#ifndef DEPTRUM_H_DT_VERSION_H_
#define DEPTRUM_H_DT_VERSION_H_

#include "dt_export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Retrieves the composite SDK version number.
 *
 * @return int Composite version value combining major, minor and patch components.
 *
 * @note The version format is: (major * 10000000) + (minor * 10000) + patch.
 */
DT_EXPORT int dt_get_version();

/**
 * @brief Retrieves the SDK version as a null-terminated string.
 * The format is "major.minor.patch-stage".
 * For example, "1.2.3-alpha".
 * The returned string has static storage duration and should not be freed.
 * @return const char* Null-terminated string representing the SDK version.
 */
DT_EXPORT const char* dt_get_version_str();

/**
 * @brief Retrieves the major component of the SDK version.
 *
 * @return int Major version number indicating breaking changes.
 */
DT_EXPORT int dt_get_major_version();

/**
 * @brief Retrieves the minor component of the SDK version.
 *
 * @return int Minor version number indicating feature additions.
 */
DT_EXPORT int dt_get_minor_version();

/**
 * @brief Retrieves the patch component of the SDK version.
 *
 * @return int Patch version number indicating bug fixes.
 */
DT_EXPORT int dt_get_patch_version();

/**
 * @brief Retrieves the development stage identifier.
 *
 * @return const char* Null-terminated string representing release stage
 *         (e.g., "alpha", "beta", "rc", "stable").
 *
 * @note The returned string has static storage duration and should not be freed.
 */
DT_EXPORT const char* dt_get_stage_version();

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_VERSION_H_