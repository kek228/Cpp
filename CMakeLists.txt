cmake_minimum_required(VERSION 3.15)
project(cppPlayground)

set(CMAKE_CXX_STANDARD 20)

include_directories(usr/local/Cellar)
link_directories(usr/local/Cellar/boost/1.72.0/lib)

find_package(Boost 1.72.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(DeducingTypes ModernCPP/DeducingTypes.cpp)
    target_link_libraries(DeducingTypes ${Boost_LIBRARIES})
endif()

#add_executable(SharedPtr SharedPtr.cpp)