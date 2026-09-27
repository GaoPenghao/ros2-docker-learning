macro(my_nav2_package)
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to Release as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
      "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
  endif()

  # Default to C++17
  if(NOT CMAKE_CXX_STANDARD)
    if ("cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
      set(CMAKE_CXX_STANDARD 17)
    else()
      message( FATAL_ERROR "cxx_std_17 could not be found.")
    endif()
  endif()

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wdeprecated -fPIC -Wshadow -Wnull-dereference)
    add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>")
  endif()
endmacro()
