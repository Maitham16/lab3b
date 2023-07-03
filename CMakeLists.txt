cmake_minimum_required(VERSION 3.12)
project(raytracing)

set(CMAKE_CXX_STANDARD 11)

# Add the pugixml library
add_subdirectory(pugixml)

# Add the stb_image library
add_library(stb_image STATIC stb_image.c)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the source files for each executable
set(MAIN_SOURCES main.cpp)

# Build each executable separately
add_executable(main ${MAIN_SOURCES})

# Link the pugixml and stb_image libraries to the main executable
target_link_libraries(main pugixml stb_image)

# Copy the necessary files to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/box.obj DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/classes DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cone.obj DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cylinder.obj DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/MarbleBeige.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Brick.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/plane_small.obj DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mramor6x6.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/rainbow.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/open_room.obj DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pugixml DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scene.dtd DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.c DESTINATION ${CMAKE_BINARY_DIR})

# Copy the scenes directory to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scenes DESTINATION ${CMAKE_BINARY_DIR})
