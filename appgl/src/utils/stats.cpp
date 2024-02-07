#include "appgl/utils/stats.hpp"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <psapi.h>
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#include <mach/mach.h>
#endif
#elif defined(__linux__)
#include <fstream>
#include <string>
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

#include <GL/glew.h>

namespace appgl::utils {

#if defined(_WIN32) || defined(_WIN64)
size_t get_memory_usage() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize; // Returns the working set size, in bytes.
    }
    return 0; // Failed to get memory info
}
#elif defined(__APPLE__)
#if TARGET_OS_MAC
size_t get_memory_usage() {
    mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS)
        return 0; // Failed to get memory info

    return info.resident_size; // Resident set size, in bytes
}
#endif
#elif defined(__linux__)
size_t get_memory_usage() {
    long pageSize = sysconf(_SC_PAGESIZE); // in bytes
    long numPages = sysconf(_SC_PHYS_PAGES);

    std::ifstream statusFile("/proc/self/statm");
    long rss = 0; // Resident Set Size
    if (statusFile) {
        statusFile >> rss >> rss; // read 'size' and 'resident' fields
        statusFile.close();
    }
    return rss * pageSize; // convert pages to bytes
}
#else
#error "Unsupported platform"
#endif

// Unreliable
size_t get_gpu_memory_usage() {
    if (GLEW_NVX_gpu_memory_info) {
        GLint totalMemoryKB = 0;
        GLint currentMemoryKB = 0;
        glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemoryKB);
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &currentMemoryKB);

        return static_cast<size_t>(totalMemoryKB - currentMemoryKB) * 1024; // in bytes
    } else if (GLEW_ATI_meminfo) {
        GLint info[4];
        glGetIntegerv(GL_TEXTURE_FREE_MEMORY_ATI, info);

        GLint totalMemoryKB = info[0];
        return static_cast<size_t>(info[1]) * 1024; // This might not be directly available; you might need to calculate based on other metrics
        // The ATI extension doesn't directly provide used memory, so you might need to infer this from other values
    } else {
        return 0; // Unsupported extension
    }
}

} // namespace appgl::utils
