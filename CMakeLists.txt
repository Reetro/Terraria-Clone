cmake_minimum_required(VERSION 3.5)

# This is required because raylib 5.0 still declares cmake_minimum_required(VERSION 3.0)
# Newer CMake versions refuse anything below 3.5. This line silences the error cleanly.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ! ! ! ! ! ! !
# Set this to ON instead of OFF to ship the game!
# When ON → production mode (console will be hidden on Windows / no terminal needed on Linux)
# When OFF → development mode (console will be visible)
# ! ! ! ! ! ! !
# Delete the build folder after changing this value.
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
	add_compile_options(/arch:AVX2)
else()
	add_compile_options(-mavx2 -mfma)        # SIMD on Linux (Cachy OS)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Raylib as static libraries" FORCE)

project(TerrariaClone)

add_subdirectory(thirdparty/raylib-5.0)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}")

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
	if(PRODUCTION_BUILD)
		# Windows only: completely hides the console window in production builds
		set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES
				LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
		)
	endif()
elseif(MINGW)
	if(PRODUCTION_BUILD)
		target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE -mwindows)  # hides console on MinGW
	endif()
elseif(APPLE OR UNIX)   # ← This branch applies to you on Cachy OS (Linux)
	# On Linux there is NO build flag that "hides" the console like Windows.
	# Console visibility is controlled by HOW you launch the executable:
	#   • Development (OFF) → run from terminal → console visible
	#   • Production (ON)  → launch without terminal (double-click or .desktop) → console hidden
	if(PRODUCTION_BUILD)
		target_link_options("${CMAKE_PROJECT_NAME}" PRIVATE -s)  # just strip symbols
	endif()
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE raylib_static)