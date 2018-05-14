
find_package(Qt5Widgets REQUIRED)
find_package(glbinding REQUIRED)

# Функция добавляет пример из курса OpenGL
function(custom_add_example TARGET MAIN_DIR)
    # Добавляем цель - исполняемый файл
    custom_add_executable_from_dirs(${TARGET} ${MAIN_DIR} ${ARGN})

    target_link_libraries(${TARGET}
        PRIVATE
        Qt5::Widgets
        glbinding::glbinding
        libs::libglcore
        libs::libmath
        libs::libplatform
    )

    custom_add_sanitizers(${TARGET})
endfunction()

custom_add_example(Sample05 "Sample05")

custom_add_example(Sample06 "Sample06")
custom_distribute_assets("Sample06" "res06" Sample06)

custom_add_example(Sample07 "Sample07")
custom_distribute_assets("Sample07" "res07" Sample07)

custom_add_example(Sample08 "Sample08")
custom_distribute_assets("Sample08" "res08" Sample08)

custom_add_example(Sample09 "Sample09")
custom_distribute_assets("Sample09" "res09" Sample09)

custom_add_example(Sample10 "Sample10")
custom_distribute_assets("Sample10/res" "res10" Sample10)
