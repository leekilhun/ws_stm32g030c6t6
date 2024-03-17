cmake_minimum_required(VERSION 3.12)
set(PRJ_NAME hx711_cmake)
project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)

set(EXECUTABLE ${PROJECT_NAME})

# 해당 폴더의 파일만 찾는다.
#file(GLOB SRC_FILES CONFIGURE_DEPENDS
  #src/hw/*.c
  #src/hw/driver/*.c
  #src/bsp/*.c
  #src/common/core/*.c
#)

# 해당 폴더를 포함해서 하위 폴더까지의 파일도 찾는다.
file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/*.S
  src/*.c  
  src/*.cpp
)

# Build the executable based on the source files
add_executable(${EXECUTABLE}  
  src/main.cpp
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
  )

target_compile_features(${EXECUTABLE} PUBLIC cxx_std_20)

target_compile_definitions(${EXECUTABLE} PRIVATE
  )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
  src 
  src/core/
  src/core/ap
  src/core/ap/_inc
  src/core/ap/cn
  src/core/ap/en
  src/core/common
  src/core/common/_inc_hw
  src/core/common/util
  src/core/hw
  src/core/sys 

  src/mcc_lib

  )

#   if (CMAKE_VERSION VERSION_GREATER 3.12)
#   set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
# endif()

#Compiler options
target_compile_options(${EXECUTABLE} PRIVATE  
  /EHsc
  /std:c++20
  /Od
  /W4  # 모든 경고 수준을 나타냅니다.
  /WX  # 경고를 오류로 처리합니다.
  )

# Linker options
# target_link_options(${EXECUTABLE} PRIVATE
  
#   -lm
#   )

