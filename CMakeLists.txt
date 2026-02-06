cmake_minimum_required(VERSION 3.20)
project(ally-lang CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND "${LLVM_DEFINITIONS}")
add_definitions(${LLVM_DEFINITIONS_LIST})
include_directories(${LLVM_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME}
  ${SOURCES}
)
target_include_directories(${PROJECT_NAME} PRIVATE
  include
)
llvm_map_components_to_libnames(LLVM_LIBS core target analysis native asmprinter codegen)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LLVM_LIBS})
