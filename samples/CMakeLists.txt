
find_package(Qt5Widgets)
find_package(glbinding REQUIRED)

# Функция добавляет пример из курса OpenGL
function(custom_add_example TARGET MAIN_DIR)
    # Добавляем цель - исполняемый файл
    custom_add_executable_from_dirs(${TARGET} ${MAIN_DIR} ${ARGN})
    # Добавляем к цели шаг копирования ресурсов
    custom_distribute_assets("res/${MAIN_DIR}" "res/${MAIN_DIR}" ${TARGET})

    target_link_libraries(${TARGET}
        PRIVATE
        Qt5::Widgets
        glbinding::glbinding
        libs::libmath
        libs::libplatform
    )

    custom_add_sanitizers(${TARGET})
endfunction()

# custom_add_example(Sample05 "Sample05")
# custom_add_example(Sample06 "Sample06")
# custom_add_example(Sample07 "Sample07")
# custom_add_example(Sample08 "Sample08")
custom_add_example(Sample09 "Sample09")
