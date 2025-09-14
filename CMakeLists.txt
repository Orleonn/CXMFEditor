cmake_minimum_required(VERSION 3.28.0)

set(PROJECT_NAME CXMFEditor)
set(PROJECT_VERSION 1.0.0)

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/build/_deps CACHE PATH "Directory for downloaded dependencies" FORCE)
include(FetchContent)
include(cmake/dependencies.cmake)



set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCE_FILES
	${SOURCE_DIR}/main.cpp
	${SOURCE_DIR}/CXMFEditor.cpp
	${SOURCE_DIR}/CXMF.cpp
	${SOURCE_DIR}/AssimpDefs.cpp
	${SOURCE_DIR}/ModelPreview.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE assimp wx::net wx::core wx::base)

if(MSVC)
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		_CRT_SECURE_NO_WARNINGS=1
		_CRT_NONSTDC_NO_WARNINGS=1
	)
	set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
