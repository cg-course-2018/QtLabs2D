
custom_add_library_from_dirs(libplatform .)
target_include_directories(libplatform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libplatform PUBLIC Qt5::Widgets)
add_library(libs::libplatform ALIAS libplatform)

qt5_wrap_cpp(libplatform_HEADERS_MOC "RenderWindow.h")
target_sources(libplatform PRIVATE ${libplatform_HEADERS_MOC})
