cmake_minimum_required(VERSION 3.5)

# This is required because raylib 5.0 still declares cmake_minimum_required(VERSION 3.0)
# Newer CMake versions refuse anything below 3.5. This line silences the error cleanly.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ! ! ! ! ! ! !
# Set this to ON instead of OFF to ship the game!
# This changes RESOURCES_PATH to the local folder next to the executable.
# ! ! ! ! ! ! !
# Delete the build folder after changing this (CMake sometimes caches it).
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

if(MSVC)
	# Corrected: proper static runtime for both Debug and Release
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
	add_compile_options(/arch:AVX2)          # SIMD optimizations
else()
	add_compile_options(-mavx2 -mfma)        # SIMD on Linux/macOS/etc. (Cachy OS uses this)
endif()

# Ensure raylib is built as a static library
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Raylib as static libraries" FORCE)

project(TerrariaClone)

# Tell CMake to build raylib, imgui, and rlImgui
add_subdirectory(thirdparty/raylib-5.0)
add_subdirectory(thirdparty/imgui-docking)
add_subdirectory(thirdparty/rlImgui)

# Collect all source files automatically
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}"
		src/gameLayer/gameMain.cpp
		src/gameLayer/gameMain.h)

# Set C++17 on the target
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD_REQUIRED ON)

if(PRODUCTION_BUILD)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC
			RESOURCES_PATH="./resources/"
			PRODUCTION_BUILD=1
	)
else()
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC
			RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
			PRODUCTION_BUILD=0
	)
endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})

if(MSVC)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
	# Console is now ALWAYS shown (removed /SUBSYSTEM:WINDOWS as requested)
elseif(MINGW)
	# Console is now ALWAYS shown (removed -mwindows as requested)
elseif(APPLE OR UNIX) # Cachy OS = Linux → this branch applies to you
	# Console is ALWAYS shown on Linux (no equivalent hiding flag needed)
	# Optional: strip symbols for smaller binary in production (harmless for console)
	if(PRODUCTION_BUILD)
		target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE -s)
	endif()
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/gameLayer")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/platform")

# Link the static raylib target
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE raylib_static imgui rlimgui)