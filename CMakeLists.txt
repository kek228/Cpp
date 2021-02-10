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
    ###
    add_executable(VariadicTemplatesFUNC  templates_fun/VariadicTemplatesFUNC.cpp)
    add_executable(VariadicTemplatesTUPLE templates_fun/VariadicTemplatesTUPLE.cpp)
    add_executable(CompileTimeFact        templates_fun/factorial.cpp)
    add_executable(metaprogramming templates_fun/MetaOperations.cpp)

    target_link_libraries(Chapter1 ${Boost_LIBRARIES})
    target_link_libraries(Chapter5 ${Boost_LIBRARIES})
    target_link_libraries(VariadicTemplatesFUNC ${Boost_LIBRARIES})
    target_link_libraries(VariadicTemplatesTUPLE ${Boost_LIBRARIES})
endif()

add_executable(SharedPtr CommonExamples/SharedPtr.h CommonExamples/SharedPtrTest.cpp)
add_executable(Const_Cast TypeCasting/Const_Cast.cpp)
add_executable(Reinterpret_Cast TypeCasting/Reinterpret_Cast.cpp)
add_executable(Static_Cast TypeCasting/Static_Cast.cpp)
add_executable(Dynamic_Cast TypeCasting/Dynamic_Cast.cpp)
add_executable(C_Style_Cast TypeCasting/C_Style_Cast.cpp)

add_executable(Playground main.cpp)