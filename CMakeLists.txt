cmake_minimum_required(VERSION 3.0)

project(CreationEngine)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
        find_package(freetype REQUIRED)
	find_package(sqlite3 CONFIG REQUIRED)
        find_package(unofficial-libuv CONFIG REQUIRED)
        include_directories(./src ./include)
ELSE()
        include_directories(./src ./include /usr/include/freetype)
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

FILE (GLOB SOURCE_FILES src/*)

IF (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY "${PROJECT_SOURCE_DIR}/resource" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	file(COPY "${PROJECT_SOURCE_DIR}/lib" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})

add_executable(CreationEngine ${SOURCE_FILES} ./include/glad.c)
 
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
        target_link_libraries(CreationEngine PRIVATE glfw sqlite3 uv freetype ${OpenCV_LIBS})
	MESSAGE(STATUS "Current system: Linux ")
ELSE ()
        target_link_libraries(CreationEngine PRIVATE ./lib/glfw3dll sqlite3 unofficial::libuv::libuv ${FREETYPE_LIBRARIES} ${OpenCV_LIBS})
	MESSAGE(STATUS "Current system: Windows ")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")


