# Get sources
file(GLOB LIBCXX_SOURCES ../src/*.cpp)
# Duetto: Remove istream/ostream users as they depend on virtual bases
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/strstream.cpp")
# Duetto: Remove pthread related stuff
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/mutex.cpp")
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/algorithm.cpp")
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/debug.cpp")
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/regex.cpp")
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/thread.cpp")
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/future.cpp")
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/condition_variable.cpp")
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/random.cpp")
# Duetto: Remove clock_gettime related stuff
list(REMOVE_ITEM LIBCXX_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../src/chrono.cpp")
if(WIN32)
  file(GLOB LIBCXX_WIN32_SOURCES ../src/support/win32/*.cpp)
  list(APPEND LIBCXX_SOURCES ${LIBCXX_WIN32_SOURCES})
endif()

# Add all the headers to the project for IDEs.
if (MSVC_IDE OR XCODE)
  file(GLOB_RECURSE LIBCXX_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/*)
  if(WIN32)
    file( GLOB LIBCXX_WIN32_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/support/win32/*.h)
    list(APPEND LIBCXX_HEADERS ${LIBCXX_WIN32_HEADERS})
  endif()
  # Force them all into the headers dir on MSVC, otherwise they end up at
  # project scope because they don't have extensions.
  if (MSVC_IDE)
    source_group("Header Files" FILES ${LIBCXX_HEADERS})
  endif()
endif()

if (LIBCXX_ENABLE_SHARED)
  add_library(cxx SHARED
    ${LIBCXX_SOURCES}
    ${LIBCXX_HEADERS}
    )
else()
  add_library(cxx STATIC
    ${LIBCXX_SOURCES}
    ${LIBCXX_HEADERS}
    )
endif()

if (DEFINED LIBCXX_CXX_ABI_DEPS)
  add_dependencies(cxx ${LIBCXX_CXX_ABI_DEPS})
endif()

# Generate library list.
set(libraries ${LIBCXX_CXX_ABI_LIBRARIES})
append_if(libraries LIBCXX_HAS_PTHREAD_LIB pthread)
append_if(libraries LIBCXX_HAS_C_LIB c)
append_if(libraries LIBCXX_HAS_M_LIB m)
append_if(libraries LIBCXX_HAS_RT_LIB rt)
append_if(libraries LIBCXX_HAS_GCC_S_LIB gcc_s)

target_link_libraries(cxx ${libraries})

# Setup flags.
append_if(compile_flags LIBCXX_HAS_FPIC_FLAG -fPIC)
append_if(link_flags LIBCXX_HAS_NODEFAULTLIBS_FLAG -nodefaultlibs)

set_target_properties(cxx
  PROPERTIES
    COMPILE_FLAGS "${compile_flags}"
    LINK_FLAGS    "${link_flags}"
    OUTPUT_NAME   "c++"
    VERSION       "1.0"
    SOVERSION     "1"
  )

install(TARGETS cxx
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

install(DIRECTORY ../include/
  DESTINATION include/c++/v1
  FILES_MATCHING
  PATTERN "*"
  PATTERN ".svn" EXCLUDE
  )
