cmake_minimum_required(VERSION 3.15)
project(cppPlayground)

set(CMAKE_CXX_STANDARD 20)

include_directories(usr/local/Cellar)
link_directories(usr/local/Cellar/boost/1.72.0/lib)

find_package(Boost 1.72.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Chapter1 ModernCPP/Chapter1.cpp)
    add_executable(Chapter5 ModernCPP/Chapter5.cpp)
    add_executable(VariadicTemplatesFUNC ModernCPP/VariadicTemplatesFUNC.cpp)
    add_executable(VariadicTemplatesTUPLE ModernCPP/VariadicTemplatesTUPLE.cpp)


    target_link_libraries(Chapter1 ${Boost_LIBRARIES})
    target_link_libraries(Chapter5 ${Boost_LIBRARIES})
    target_link_libraries(VariadicTemplatesFUNC ${Boost_LIBRARIES})
    target_link_libraries(VariadicTemplatesTUPLE ${Boost_LIBRARIES})
endif()

add_executable(SharedPtr CommonExamples/SharedPtr.h CommonExamples/SharedPtrTest.cpp)

#add_executable(SharedPtr SharedPtr.cpp)