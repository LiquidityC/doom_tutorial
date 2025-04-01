cmake_minimum_required(VERSION 3.7)

project(sdl_doom LANGUAGES C VERSION 0.1.1)

# Compile and link flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

include(FetchContent)
FetchContent_Declare(
	SDL
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.2.10
	GIT_SHALLOW true
	GIT_PROGRESS true
)

message(STATUS "Fetching SDL")
FetchContent_MakeAvailable(SDL)
set_property(DIRECTORY "${sdl_SOURCE_DIR}" PROPERTY EXCLUDE_FROM_ALL TRUE)

# Environment defines
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	set(DEBUG 1)
endif ()

function (create_executable name)
	add_executable(${name}
		src/common.c
		src/${name}.c
		)
	target_include_directories(${name}
		PRIVATE include
		)
		target_link_libraries(${name} SDL3::SDL3 m)
endfunction ()

create_executable(topdown)
create_executable(topdown_centered)
create_executable(topdown_centered_rotation)
