cmake_minimum_required(VERSION 3.4)
project(TLinkage)
list(APPEND CMAKE_FRAMEWORK_PATH /Library/Frameworks) #Mac, why not auto?

find_package(Imagine REQUIRED)


add_executable(
    tlk TLinkage.cpp
    point.h
    point.cpp
    line.h
    line.cpp
    window.h
    window.cpp
    cluster.h
    cluster.cpp
    settings.h
    model.h
    model.cpp
    util.h
    util.cpp)
ImagineUseModules(tlk Graphics)
