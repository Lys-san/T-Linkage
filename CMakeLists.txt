cmake_minimum_required(VERSION 3.4)
project(TLinkage)
list(APPEND CMAKE_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?

find_package(Imagine REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenMP)

if(${OpenCV_VERSION} VERSION_LESS 2.0.0)
    message(FATAL_ERROR “OpenCV version is not compatible : ${OpenCV_VERSION}”)
endif()


include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(tlk ${all_SRCS})

ImagineUseModules(tlk Graphics)
target_link_libraries(tlk ${OpenCV_LIBS})

if(OpenMP_CXX_FOUND)
    target_link_libraries(tlk OpenMP::OpenMP_CXX)
endif()
