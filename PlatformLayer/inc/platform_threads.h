/**
 * @file platform_threads.h
 * @brief Platform-agnostic thread management interface
 */
#ifndef PLATFORM_THREADS_H
#define PLATFORM_THREADS_H

// 1. System includes
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// 2. Platform-specific includes
#include "platform_error.h"
// No way around this
#ifdef _WIN32
    #include <windows.h>
    #define THREAD_LOCAL __declspec(thread)
#else
    #include <pthread.h>
    #define THREAD_LOCAL __thread
#endif

// platform agnostic thread handle
typedef void* PlatformThreadHandle;

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Thread priority levels
 */
typedef enum {
    PLATFORM_THREAD_PRIORITY_LOWEST = -2,
    PLATFORM_THREAD_PRIORITY_LOW = -1,
    PLATFORM_THREAD_PRIORITY_NORMAL = 0,
    PLATFORM_THREAD_PRIORITY_HIGH = 1,
    PLATFORM_THREAD_PRIORITY_HIGHEST = 2,
    PLATFORM_THREAD_PRIORITY_REALTIME = 3
} PlatformThreadPriority;

/**
 * @brief Thread attributes structure
 */
typedef struct {
    PlatformThreadPriority priority;  ///< Thread priority
    size_t stack_size;                ///< Stack size in bytes (0 for default)
    bool detached;                    ///< True if thread should be detached
} PlatformThreadAttributes;

/**
 * @brief Thread function prototype
 */
typedef void* (*PlatformThreadFunction)(void* arg);

/**
 * @brief Opaque thread handle type
 */
typedef void* PlatformThreadHandle;

/**
 * @brief Opaque thread ID type
 */
typedef uintptr_t PlatformThreadId;

/**
 * @brief Initialize the threading subsystem
 * @return PlatformErrorCode indicating success or failure
 */
PlatformErrorCode platform_thread_init(void);

/**
 * @brief Cleanup the threading subsystem
 */
void platform_thread_cleanup(void);

/**
 * @brief Create a new thread
 * @param[out] thread_id Thread id
 * @param[in] attributes Thread attributes (NULL for defaults)
 * @param[in] function Thread function to execute
 * @param[in] arg Argument to pass to thread function
 * @return PlatformErrorCode indicating success or failure
 */
PlatformErrorCode platform_thread_create(
    PlatformThreadId* thread_id,
    const PlatformThreadAttributes* attributes,
    PlatformThreadFunction function,
    void* arg);

/**
 * @brief Join with a thread (wait for it to complete)
 * @param[in] handle Thread handle
 * @param[out] result Thread return value
 * @return PlatformErrorCode indicating success or failure
 */
PlatformErrorCode platform_thread_join(PlatformThreadHandle handle, void** result);

/**
 * @brief Detach a thread
 * @param[in] handle Thread handle
 * @return PlatformErrorCode indicating success or failure
 */
PlatformErrorCode platform_thread_detach(PlatformThreadHandle handle);

/**
 * @brief Get current thread ID
 * @return Current thread ID
 */
PlatformThreadId platform_thread_get_id(void);

/**
 * @brief Get current thread handle
 * @return Current thread handle
 */
PlatformThreadHandle platform_thread_get_handle(void);

/**
 * @brief Set thread priority
 * @param[in] handle Thread handle
 * @param[in] priority New priority level
 * @return PlatformErrorCode indicating success or failure
 */
PlatformErrorCode platform_thread_set_priority(
    PlatformThreadHandle handle,
    PlatformThreadPriority priority);

/**
 * @brief Get thread priority
 * @param[in] handle Thread handle
 * @param[out] priority Current priority level
 * @return PlatformErrorCode indicating success or failure
 */
PlatformErrorCode platform_thread_get_priority(
    PlatformThreadHandle handle,
    PlatformThreadPriority* priority);

/**
 * @brief Yield execution to another thread
 */
void platform_thread_yield(void);

typedef enum PlatformThreadStatus {
    PLATFORM_THREAD_ALIVE,      // Thread is running
    PLATFORM_THREAD_DEAD,       // Thread no longer exists
    PLATFORM_THREAD_TERMINATED, // Thread has terminated normally
    PLATFORM_THREAD_UNKNOWN     // Status cannot be determined
} PlatformThreadStatus;

/**
 * Get the current status of a thread
 * 
 * @param thread_id The platform-specific thread identifier
 * @param status Pointer to store the thread status
 * @return PLATFORM_ERROR_SUCCESS if status was retrieved successfully
 *         PLATFORM_ERROR_INVALID_ARGUMENT if status is NULL
 *         PLATFORM_ERROR_UNKNOWN if platform-specific error occurred
 */
PlatformErrorCode platform_thread_get_status(PlatformThreadId thread_id, PlatformThreadStatus* status);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_THREADS_H
