# CMakeList.txt : CMake project for appgl, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library(appgl STATIC
     "src/ui/imgui_nodes.cpp"
     "src/ui/imgui_theme.cpp"
     "src/ui/imgui_widgets.cpp"
     "src/utils/stats.cpp"
     "src/display.cpp"
     "src/gl/gl.cpp"
     "src/gui.cpp"
     "src/logging.cpp"
     "src/platform.cpp"
     "src/stb.cpp"
     "src/window.cpp"
)
target_include_directories(appgl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(appgl PRIVATE $<$<CONFIG:Debug>:DEBUG_OPENGL>)

set_property(TARGET appgl PROPERTY CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

# Include the subdirectories for each library
target_compile_definitions(appgl PUBLIC SPDLOG_COMPILED_LIB)
add_subdirectory(${COMMON_LIBS_DIR}/spdlog spdlog)
get_target_property(spdlog_includes spdlog INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(appgl INTERFACE ${spdlog_includes})

add_subdirectory(${COMMON_LIBS_DIR}/glm glm)
get_target_property(glm_includes glm INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(appgl INTERFACE ${glm_includes})

add_subdirectory(${COMMON_LIBS_DIR}/glfw glfw)
get_target_property(glfw_includes glfw INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(appgl INTERFACE ${glfw_includes})

add_subdirectory(${COMMON_LIBS_DIR}/nativefiledialog-extended nfd)
get_target_property(nfd_includes nfd INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(appgl INTERFACE ${nfd_includes})

# Set ONLY_LIBS to ON before including the subdirectory
set(ONLY_LIBS ON CACHE BOOL "Build only libraries")
add_subdirectory(${COMMON_LIBS_DIR}/glew-cmake glew)
get_target_property(glew_includes libglew_static INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(appgl INTERFACE ${glew_includes})
set(ONLY_LIBS OFF CACHE BOOL "Build only libraries" FORCE)

# Add imgui source files
set(IMGUI_DIR ${COMMON_LIBS_DIR}/imgui)
file(GLOB IMGUI_SOURCES 
     ${IMGUI_DIR}/imgui.cpp 
     ${IMGUI_DIR}/imgui_draw.cpp 
     ${IMGUI_DIR}/imgui_widgets.cpp 
     ${IMGUI_DIR}/imgui_tables.cpp
     ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
     ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

# Add imgui headers
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_include_directories(appgl INTERFACE ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# Add stb headers
set(STB_HEADERS ${COMMON_LIBS_DIR}/stb)
include_directories(appgl INTERFACE ${STB_HEADERS})
target_include_directories(appgl INTERFACE ${STB_HEADERS})

# Link the libraries to your project
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL spdlog::spdlog glm glfw libglew_static nfd)
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES})
