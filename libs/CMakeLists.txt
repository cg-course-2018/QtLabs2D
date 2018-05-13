
find_package(Qt5Widgets REQUIRED)
find_package(glbinding REQUIRED)

add_subdirectory(libplatform)

custom_add_library_from_dirs(libglcore libglcore)
target_include_directories(libglcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libglcore PUBLIC glbinding::glbinding)
target_compile_definitions(libglcore PUBLIC GLM_ENABLE_EXPERIMENTAL)
add_library(libs::libglcore ALIAS libglcore)

custom_add_library_from_dirs(libmath libmath)
target_include_directories(libmath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(libmath PUBLIC GLM_ENABLE_EXPERIMENTAL)
add_library(libs::libmath ALIAS libmath)

custom_add_library_from_dirs(tinyxml2 tinyxml2)
target_include_directories(tinyxml2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(libs::tinyxml2 ALIAS tinyxml2)
