get_filename_component(ID ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ID ${ID})

project(${ID})

set(Dependencies MetisInterface)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source/*.cpp)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_sources})

foreach(Dependency ${Dependencies})
    target_link_libraries(${PROJECT_NAME} ${Dependency})
endforeach()
