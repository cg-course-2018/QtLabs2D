cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

# Макрос включает статическую компоновку C++ Runtime при сборке с Visual Studio.
macro(custom_use_static_msvc_runtime)
    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
    endif(MSVC)
endmacro()

# В текущей версии CMake не может включить режим C++17 в некоторых компиляторах.
# Функция использует обходной манёвр.
function(custom_enable_cxx17 TARGET)
    # Включаем C++17 везде, где CMake может.
	target_compile_features(${TARGET} PUBLIC cxx_std_17)
    # Включаем режим C++latest в Visual Studio
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "/std:c++latest")
    # Включаем компоновку с libc++, libc++experimental и pthread для Clang
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "-stdlib=libc++ -pthread")
        target_link_libraries(${TARGET} c++experimental pthread)
    endif()
endfunction(custom_enable_cxx17)

function(custom_clang_tidy_checks)
    set(CLANG_TIDY_OPTIONS "clang-tidy;-checks=")
    set(SEPARATOR "")
    foreach(CHECK ${ARGN})
        set(CLANG_TIDY_OPTIONS "${CLANG_TIDY_OPTIONS}${SEPARATOR}${CHECK}")
        set(SEPARATOR ",")
    endforeach()
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS} PARENT_SCOPE)
endfunction(custom_clang_tidy_checks)

# Функция добавляет вспомогательную библиотеку из курса OpenGL.
# Использование: custom_add_library_from_dirs(myapp . ./detail ./helpers)
function(custom_add_library_from_dirs TARGET)
    # Собираем файлы с указанных каталогов
    foreach(DIR ${ARGN})
        file(GLOB TARGET_SRC_PART "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.h")
        list(APPEND TARGET_SRC ${TARGET_SRC_PART})
    endforeach()
    # Добавляем цель - библиотеку
    add_library(${TARGET} ${TARGET_SRC})
    # Включаем режим C++17
    custom_enable_cxx17(${TARGET})
endfunction()

# Функция добавляет пример из курса OpenGL
function(custom_add_executable_from_dirs TARGET)
    # Собираем файлы с указанных каталогов
    foreach(DIR ${ARGN})
        file(GLOB TARGET_SRC_PART "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*.h")
        list(APPEND TARGET_SRC ${TARGET_SRC_PART})
    endforeach()
    # Добавляем цель - исполняемый файл
    add_executable(${TARGET} ${TARGET_SRC})
    # Включаем режим C++17
    custom_enable_cxx17(${TARGET})
endfunction()

# Функция включает Address Sanitizer и UB Sanitizer для компиляторов, где они доступны.
function(custom_add_sanitizers TARGET)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${TARGET} PRIVATE "-fsanitize=address" "-fsanitize=undefined")
        target_link_libraries(${TARGET} "-fsanitize=address" "-fsanitize=undefined")
    endif()
endfunction()

# Функция копирует файлы из каталога в другой каталог,
#  но только в случае, если файл не был скопирован ранее
#  либо изменился (обновилась дата модификации).
function(custom_distribute_assets SOURCE_DIRNAME DESTINATION_DIRNAME TARGET)
    # Собираем список файлов с расширениями, характерными для ресурсов.
    set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_DIRNAME}")
    set(DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${DESTINATION_DIRNAME}")
    file(GLOB_RECURSE SRC_FILES
        "${SOURCE}/*.png"
        "${SOURCE}/*.jpg"
        "${SOURCE}/*.vert"
        "${SOURCE}/*.frag")

    # FROM содержить путь к исходному файлу, TO содержит целевой путь.
    foreach(FROM ${SRC_FILES})
        string(REPLACE "${SOURCE}" "${DESTINATION}" TO "${FROM}")
        add_custom_command(
                   TARGET ${TARGET}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND}
                   ARGS    -E copy_if_different ${FROM} ${TO}
                   COMMENT "Copying ${FROM} ${DESTINATION}"
                   )
    endforeach()
endfunction()
