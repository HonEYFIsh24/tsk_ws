/**
 * @file dt_frame.h
 * @brief Functions for frame data and information management.
 */

#ifndef DEPTRUM_H_DT_FRAME_H_
#define DEPTRUM_H_DT_FRAME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Creates a generic frame object.
 *
 * @attention Created frame has reference count=1. Decrease via @ref dt_delete_frame() after use.
 * @param[in] frame_type Type of the frame object.
 * @param[in] format Data format of the frame.
 * @param[in] data_size Size of frame data buffer in bytes.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Pointer to created frame object.
 */
DT_EXPORT dt_frame* dt_create_frame(dt_frame_type frame_type,
                                    dt_format format,
                                    uint32_t data_size,
                                    dt_error** error);

/**
 * @brief Creates a video frame with specified dimensions.
 *
 * @attention Created frame has reference count=1. Decrease via @ref dt_delete_frame() after use.
 * @param[in] frame_type Type of the frame object.
 * @param[in] format Pixel format of the frame.
 * @param[in] width Frame width in pixels.
 * @param[in] height Frame height in pixels.
 * @param[in] stride_bytes Row stride in bytes (0=auto-calculate).
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Pointer to created video frame.
 */
DT_EXPORT dt_frame* dt_create_video_frame(dt_frame_type frame_type,
                                          dt_format format,
                                          uint32_t width,
                                          uint32_t height,
                                          uint32_t stride_bytes,
                                          dt_error** error);

/**
 * @brief Creates a frame from an external buffer.
 *
 * @attention User retains ownership of buffer. Destroy callback will be invoked on frame deletion.
 * @attention Created frame has reference count=1. Decrease via @ref dt_delete_frame() after use.
 * @param[in] frame_type Type of the frame object.
 * @param[in] format Data format of the frame.
 * @param[in] buffer External data buffer.
 * @param[in] buffer_size Size of buffer in bytes.
 * @param[in] buffer_destroy_cb Callback invoked when frame is destroyed.
 * @param[in] buffer_destroy_context User context for destroy callback.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Pointer to created frame object.
 */
DT_EXPORT dt_frame* dt_create_frame_from_buffer(dt_frame_type frame_type,
                                                dt_format format,
                                                uint8_t* buffer,
                                                uint32_t buffer_size,
                                                dt_frame_destroy_callback* buffer_destroy_cb,
                                                void* buffer_destroy_context,
                                                dt_error** error);

/**
 * @brief Creates a video frame from an external buffer.
 *
 * @attention User retains ownership of buffer. Destroy callback will be invoked on frame deletion.
 * @attention Created frame has reference count=1. Decrease via @ref dt_delete_frame() after use.
 * @param[in] frame_type Type of the frame object.
 * @param[in] format Pixel format of the frame.
 * @param[in] width Frame width in pixels.
 * @param[in] height Frame height in pixels.
 * @param[in] stride_bytes Row stride in bytes (0=auto-calculate).
 * @param[in] buffer External data buffer.
 * @param[in] buffer_size Size of buffer in bytes.
 * @param[in] buffer_destroy_cb Callback invoked when frame is destroyed.
 * @param[in] buffer_destroy_context User context for destroy callback.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Pointer to created video frame.
 */
DT_EXPORT dt_frame* dt_create_video_frame_from_buffer(dt_frame_type frame_type,
                                                      dt_format format,
                                                      uint32_t width,
                                                      uint32_t height,
                                                      uint32_t stride_bytes,
                                                      uint8_t* buffer,
                                                      uint32_t buffer_size,
                                                      dt_frame_destroy_callback* buffer_destroy_cb,
                                                      void* buffer_destroy_context,
                                                      dt_error** error);

/**
 * @brief Clones a frame with optional data copy.
 *
 * @attention Created frame has reference count=1. Decrease via @ref dt_delete_frame() after use.
 * @param[in] other_frame Source frame to clone.
 * @param[in] should_copy_data True to copy data, false for uninitialized buffer.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* New frame with duplicated properties.
 */
DT_EXPORT dt_frame* dt_create_frame_from_other_frame(const dt_frame* other_frame,
                                                     bool should_copy_data,
                                                     dt_error** error);

/**
 * @brief Creates a frame based on stream profile specifications.
 *
 * @attention Created frame has reference count=1. Decrease via @ref dt_delete_frame() after use.
 * @param[in] stream_profile Stream profile defining frame parameters.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Newly created frame object.
 */
DT_EXPORT dt_frame* dt_create_frame_from_stream_profile(const dt_stream_profile* stream_profile,
                                                        dt_error** error);

/**
 * @brief Creates an empty frameset container.
 *
 * @attention Created frameset has reference count=1. Decrease via @ref dt_delete_frame() after use.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Pointer to new frameset object.
 */
DT_EXPORT dt_frame* dt_create_frameset(dt_error** error);

/**
 * @brief Decrements frame reference count and destroys if unreferenced.
 *
 * @param[in] frame Frame to release references to.
 */
DT_EXPORT void dt_delete_frame(const dt_frame* frame);

/**
 * @brief Increases frame reference count.
 *
 * @attention Each call requires matching @ref dt_delete_frame() to release.
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_frame_add_ref(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves frame metadata buffer.
 *
 * @warning Modifying buffer during multi-threaded use causes undefined behavior.
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint8_t* Mutable pointer to metadata buffer.
 */
DT_EXPORT uint8_t* dt_frame_get_metadata(const dt_frame* frame, dt_error** error);

/**
 * @brief Gets size of frame metadata.
 *
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Metadata size in bytes.
 */
DT_EXPORT uint32_t dt_frame_get_metadata_size(const dt_frame* frame, dt_error** error);

/**
 * @brief Checks for existence of specified metadata type.
 *
 * @param[in] frame Target frame object.
 * @param[in] type Metadata type to query.
 * @param[out] error Receives detailed error information if operation fails.
 * @return bool True if metadata type exists.
 */
DT_EXPORT bool dt_frame_has_metadata(const dt_frame* frame,
                                     dt_frame_metadata_type type,
                                     dt_error** error);

/**
 * @brief Retrieves integer value of specified metadata.
 *
 * @param[in] frame Target frame object.
 * @param[in] type Metadata type to retrieve.
 * @param[out] error Receives detailed error information if operation fails.
 * @return int64_t Value of requested metadata.
 */
DT_EXPORT int64_t dt_frame_get_metadata_value(const dt_frame* frame,
                                              dt_frame_metadata_type type,
                                              dt_error** error);

/**
 * @brief Replaces frame metadata content.
 *
 * @warning Buffer must not exceed 256 bytes to prevent memory exceptions.
 * @param[in] frame Target frame object.
 * @param[in] metadata New metadata buffer.
 * @param[in] metadata_size Size of new metadata in bytes.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_frame_update_metadata(dt_frame* frame,
                                        const uint8_t* metadata,
                                        uint32_t metadata_size,
                                        dt_error** error);

/**
 * @brief Retrieves frame index identifier.
 *
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint64_t Unique frame index.
 */
DT_EXPORT uint64_t dt_frame_get_index(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves frame data buffer.
 *
 * @warning Direct modification during multi-threaded use causes undefined behavior.
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint8_t* Mutable pointer to frame data.
 */
DT_EXPORT uint8_t* dt_frame_get_data(const dt_frame* frame, dt_error** error);

/**
 * @brief Gets size of frame data buffer.
 *
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Size of data buffer in bytes.
 */
DT_EXPORT uint32_t dt_frame_get_data_size(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves frame type classification.
 *
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame_type Frame type identifier.
 */
DT_EXPORT dt_frame_type dt_frame_get_type(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves frame data format.
 *
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_format Data format identifier.
 */
DT_EXPORT dt_format dt_frame_get_format(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves frame name string.
 *
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return const char* Frame name (NULL if unset).
 */
DT_EXPORT const char* dt_frame_get_name(const dt_frame* frame, dt_error** error);

/**
 * @brief Sets device/hardware timestamp (microseconds).
 *
 * @param[in] frame Target frame object.
 * @param[in] timestamp_us Timestamp in microseconds.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_frame_set_timestamp_us(dt_frame* frame, uint64_t timestamp_us, dt_error** error);

/**
 * @brief Retrieves device/hardware timestamp (microseconds).
 *
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint64_t Captured timestamp in microseconds.
 */
DT_EXPORT uint64_t dt_frame_get_timestamp_us(const dt_frame* frame, dt_error** error);

/**
 * @brief Sets host reception timestamp (microseconds).
 *
 * @param[in] frame Target frame object.
 * @param[in] system_timestamp_us Host timestamp in microseconds.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_frame_set_system_timestamp_us(dt_frame* frame,
                                                uint64_t system_timestamp_us,
                                                dt_error** error);

/**
 * @brief Retrieves host reception timestamp (microseconds).
 *
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint64_t Host timestamp in microseconds.
 */
DT_EXPORT uint64_t dt_frame_get_system_timestamp_us(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves drift-corrected global timestamp (microseconds).
 *
 * @attention Requires prior call to @ref dt_device_enable_global_timestamp().
 * @attention Functionality depends on device capabilities.
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint64_t Global timestamp in microseconds (0 if unsupported).
 */
DT_EXPORT uint64_t dt_frame_get_global_timestamp_us(const dt_frame* frame, dt_error** error);

/**
 * @brief Replaces frame data content.
 *
 * @warning Input size must not exceed original frame data size.
 * @param[in] frame Target frame object.
 * @param[in] data New data buffer.
 * @param[in] data_size Size of new data in bytes.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_frame_update_data(dt_frame* frame,
                                    const uint8_t* data,
                                    uint32_t data_size,
                                    dt_error** error);

/**
 * @brief Copies informational fields between frames.
 *
 * @details Copies index, timestamps, and metadata.
 * @param[in] src_frame Source frame for copying.
 * @param[in] dst_frame Destination frame for copying.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_frame_copy_info(const dt_frame* src_frame, dt_frame* dst_frame, dt_error** error);

/**
 * @brief Assigns stream profile to frame.
 *
 * @param[in] frame Target frame object.
 * @param[in] stream_profile Stream profile to associate.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_frame_set_stream_profile(dt_frame* frame,
                                           const dt_stream_profile* stream_profile,
                                           dt_error** error);

/**
 * @brief Retrieves associated stream profile.
 *
 * @attention Returned object requires @ref dt_delete_stream_profile() for cleanup.
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* Associated stream profile (NULL if unset).
 */
DT_EXPORT dt_stream_profile* dt_frame_get_stream_profile(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves source sensor object.
 *
 * @attention Returned object requires @ref dt_delete_sensor() for cleanup.
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_sensor* Source sensor object (NULL if unavailable).
 */
DT_EXPORT dt_sensor* dt_frame_get_sensor(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves source device object.
 *
 * @attention Returned object requires @ref dt_delete_device() for cleanup.
 * @param[in] frame Target frame object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_device* Source device object (NULL if unavailable).
 */
DT_EXPORT dt_device* dt_frame_get_device(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves video frame width.
 *
 * @param[in] frame Target video frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Frame width in pixels.
 */
DT_EXPORT uint32_t dt_video_frame_get_width(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves video frame height.
 *
 * @param[in] frame Target video frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Frame height in pixels.
 */
DT_EXPORT uint32_t dt_video_frame_get_height(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves effective bits per pixel.
 *
 * @attention Only valid for Y8/Y10/Y11/Y12/Y14/Y16 formats.
 * @param[in] frame Target video frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint8_t Effective bits per pixel (0 if unsupported).
 */
DT_EXPORT uint8_t dt_video_frame_get_pixel_available_bit_size(const dt_frame* frame,
                                                              dt_error** error);

/**
 * @brief Sets effective bits per pixel.
 *
 * @attention Only valid for Y8/Y10/Y11/Y12/Y14/Y16 formats.
 * @param[in] frame Target video frame.
 * @param[in] bit_size Effective bits to assign.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_video_frame_set_pixel_available_bit_size(dt_frame* frame,
                                                           uint8_t bit_size,
                                                           dt_error** error);

/**
 * @brief Identifies IR sensor source.
 *
 * @param[in] frame Target IR frame.
 * @param[out] dt_error Receives detailed error information if operation fails.
 * @return dt_sensor_type Source sensor identifier.
 */
DT_EXPORT dt_sensor_type dt_ir_frame_get_source_sensor_type(const dt_frame* frame,
                                                            dt_error** dt_error);

/**
 * @brief Retrieves depth value scaling factor.
 *
 * @details Depth in mm = pixel_value * scale.
 * @param[in] frame Target depth frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return float Current scaling factor.
 */
DT_EXPORT float dt_depth_frame_get_value_scale(const dt_frame* frame, dt_error** error);

/**
 * @brief Sets depth value scaling factor.
 *
 * @details Depth in mm = pixel_value * scale.
 * @param[in] frame Target depth frame.
 * @param[in] value_scale Scaling factor to apply.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_depth_frame_set_value_scale(dt_frame* frame, float value_scale, dt_error** error);

/**
 * @brief Retrieves coordinate scaling factor.
 *
 * @details Position in mm = coordinate_value * scale.
 * @param[in] frame Target points frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return float Current coordinate scaling factor.
 */
DT_EXPORT float dt_points_frame_get_coordinate_value_scale(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves accelerometer data.
 *
 * @param[in] frame Target accelerometer frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_accel_value Accelerometer data values.
 */
DT_EXPORT dt_accel_value dt_accel_frame_get_value(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves accelerometer temperature.
 *
 * @param[in] frame Target accelerometer frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return float Temperature value in Celsius.
 */
DT_EXPORT float dt_accel_frame_get_temperature(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves gyroscope data.
 *
 * @param[in] frame Target gyroscope frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_gyro_value Gyroscope data values.
 */
DT_EXPORT dt_gyro_value dt_gyro_frame_get_value(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves gyroscope temperature.
 *
 * @param[in] frame Target gyroscope frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return float Temperature value in Celsius.
 */
DT_EXPORT float dt_gyro_frame_get_temperature(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves the width dimension of a point cloud frame.
 *
 * @param[in] frame Point cloud frame instance (must be DT_FRAME_TYPE_POINTS).
 * @param[out] error Receives detailed error information if operation fails.
 * @return Horizontal resolution (number of points per row).
 */
DT_EXPORT uint32_t dt_point_cloud_frame_get_width(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves the height dimension of a point cloud frame.
 *
 * @param[in] frame Point cloud frame instance (must be DT_FRAME_TYPE_POINTS).
 * @param[out] error Receives detailed error information if operation fails.
 * @return Vertical resolution (number of point rows).
 */
DT_EXPORT uint32_t dt_point_cloud_frame_get_height(const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves frame by type from frameset.
 *
 * @attention Returned frame requires @ref dt_delete_frame() for cleanup.
 * @param[in] frameset Target frameset container.
 * @param[in] frame_type Type of frame to retrieve.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Requested frame (NULL if not found).
 */
DT_EXPORT dt_frame* dt_frameset_get_frame(const dt_frame* frameset,
                                          dt_frame_type frame_type,
                                          dt_error** error);

/**
 * @brief Retrieves frame by index from frameset.
 *
 * @param[in] frameset Target frameset container.
 * @param[in] index Position index in frameset.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Requested frame (NULL if index invalid).
 */
DT_EXPORT dt_frame* dt_frameset_get_frame_by_index(const dt_frame* frameset,
                                                   uint32_t index,
                                                   dt_error** error);

/**
 * @brief Retrieves frame by name from frameset.
 *
 * @param[in] frameset Target frameset container.
 * @param[in] name Name identifier of frame.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_frame* Requested frame (NULL if not found).
 */
DT_EXPORT dt_frame* dt_frameset_get_frame_by_name(const dt_frame* frameset,
                                                  const char* name,
                                                  dt_error** error);

/**
 * @brief Adds frame to frameset container.
 *
 * @attention Replaces existing frames with same type.
 * @attention Increments frame's reference count.
 * @param[in] frameset Target frameset container.
 * @param[in] frame Frame to add.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_frameset_push_frame(dt_frame* frameset, const dt_frame* frame, dt_error** error);

/**
 * @brief Retrieves the number of frames contained in a frameset.
 *
 * @param[in] frameset Frameset container instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Frame count within the frameset.
 */
DT_EXPORT uint32_t dt_frameset_get_count(const dt_frame* frameset, dt_error** error);

/**
 * @brief Extracts the depth frame from a frameset.
 *
 * Increases frame reference count automatically. Caller must release with dt_frame_release().
 *
 * @param[in] frameset Frameset container instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Depth frame handle. Valid until explicitly released.
 */
DT_EXPORT dt_frame* dt_frameset_get_depth_frame(const dt_frame* frameset, dt_error** error);

/**
 * @brief Extracts the color frame from a frameset.
 *
 * Increases frame reference count automatically. Caller must release with dt_frame_release().
 *
 * @param[in] frameset Frameset container instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Color frame handle. Valid until explicitly released.
 */
DT_EXPORT dt_frame* dt_frameset_get_color_frame(const dt_frame* frameset, dt_error** error);

/**
 * @brief Extracts the infrared frame from a frameset.
 *
 * Increases frame reference count automatically. Caller must release with dt_frame_release().
 *
 * @param[in] frameset Frameset container instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Infrared frame handle. Valid until explicitly released.
 */
DT_EXPORT dt_frame* dt_frameset_get_ir_frame(const dt_frame* frameset, dt_error** error);

/**
 * @brief Extracts the point cloud frame from a frameset.
 *
 * Increases frame reference count automatically. Caller must release with dt_frame_release().
 *
 * @param[in] frameset Frameset container instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Point cloud frame handle. Valid until explicitly released.
 */
DT_EXPORT dt_frame* dt_frameset_get_points_frame(const dt_frame* frameset, dt_error** error);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_FRAME_H_