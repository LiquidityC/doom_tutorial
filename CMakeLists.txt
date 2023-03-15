cmake_minimum_required(VERSION 3.7)

project(sdl_doom LANGUAGES C VERSION 0.1.0)

# Compile and link flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

# Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

include(FindSDL2)
set(SDL_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS})
set(SDL_LIBRARIES ${SDL2_LIBRARIES})

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
		PRIVATE ${SDL_INCLUDE_DIRS}
		PRIVATE include
		)
	target_link_libraries(${name} ${SDL_LIBRARIES} m)
endfunction ()

create_executable(topdown)
create_executable(topdown_centered)
create_executable(topdown_centered_rotation)
