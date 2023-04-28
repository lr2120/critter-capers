cmake_minimum_required(VERSION 3.15)
project(Critters)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

add_executable(Critters main.cpp processData.cpp processData.h Graph.cpp Graph.h BreadthFirstSearch.cpp BreadthFirstSearch.h DepthFirstSearch.cpp DepthFirstSearch.h sfml.cpp sfml.h ForceDirectedGraph.cpp ForceDirectedGraph.h Species.cpp Species.h)

target_compile_options(Critters PUBLIC -fsanitize=address -fno-omit-frame-pointer)
target_link_options(Critters PUBLIC -fsanitize=address)

target_include_directories(Critters PRIVATE
        ${SFML_INCLUDE_DIR}
        )

target_link_libraries(Critters
        sfml-graphics sfml-audio sfml-window sfml-system
        )
