cmake_minimum_required(VERSION 3.12)
project(hx711_cmake
  LANGUAGES ASM C CXX
)

set(EXECUTABLE ${PROJECT_NAME})

# 해당 폴더를 포함해서 하위 폴더까지의 파일도 찾는다.
file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/*.S
  src/*.c  
  src/*.cpp
)

# Build the executable based on the source files
add_executable(${EXECUTABLE}  
  ${SRC_FILES_RECURSE}
  )

# C++20 표준을 사용합니다.
target_compile_features(${EXECUTABLE} PUBLIC cxx_std_20)

# List of include directories
target_include_directories(${EXECUTABLE} PRIVATE
  src/ 
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

#Compiler options
target_compile_options(${EXECUTABLE} PRIVATE  
  /EHsc
  /std:c++20
  /Od
  #/W4  # 모든 경고 수준을 나타냅니다.
  #/WX  # 경고를 오류로 처리합니다.
  )

# Windows 플랫폼을 대상으로 할 때와 다른 플랫폼을 대상으로 할 때를 구분하여 컴파일러 옵션을 설정해야 할 수 있습니다.
# 예를 들어, GCC나 Clang을 사용하는 경우 아래와 같은 조건문을 사용할 수 있습니다.
if(NOT MSVC)
  target_compile_options(${EXECUTABLE} PRIVATE
    -Wall -Wextra -pedantic -Werror
  )
endif()

# Linker options
# target_link_libraries(${EXECUTABLE} PRIVATE
#   여기에 필요한 라이브러리 링크 옵션을 추가합니다.
#   예: pthread, m (수학 라이브러리), 여기에 추가적인 라이브러리
# )