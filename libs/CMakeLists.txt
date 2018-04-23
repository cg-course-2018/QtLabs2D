
custom_add_library_from_dirs(libglcore libglcore)
target_include_directories(libglcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(libs::libglcore ALIAS libglcore)

custom_add_library_from_dirs(libplatform libglcore)
target_include_directories(libplatform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(libs::libplatform ALIAS libglcore)

custom_add_library_from_dirs(libmath libglcore)
target_include_directories(libmath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(libs::libmath ALIAS libglcore)
