lgd_generate_sourcelists("./")
set(RELATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/")
include(auto.cmake)

if(WITH_HPX)
  add_hpx_executable(gameoflife_hpx
                     SOURCES ${SOURCES})

  target_link_libraries(gameoflife_hpx_exe ${LOCAL_LIBGEODECOMP_LINK_LIB})
endif()
