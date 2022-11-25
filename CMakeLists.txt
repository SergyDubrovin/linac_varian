cmake_minimum_required(VERSION 3.10...3.21)
project(linac_prog)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB header ${PROJECT_SOURCE_DIR}/include/*.hh)

set(LINAC_SCRIPTS
  vis.mac
  )
  foreach(_script ${LINAC_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

add_executable(linac_prog main.cc ${sources} ${headers})
target_link_libraries(linac_prog ${Geant4_LIBRARIES})
