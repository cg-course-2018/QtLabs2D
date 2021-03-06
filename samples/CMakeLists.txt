
find_package(Qt5Widgets REQUIRED)
find_package(glbinding REQUIRED)
find_package(PCL REQUIRED)
find_package(Eigen3 REQUIRED)

# Функция добавляет пример из курса OpenGL
function(custom_add_example TARGET MAIN_DIR)
    # Добавляем цель - исполняемый файл
    custom_add_executable_from_dirs(${TARGET} ${MAIN_DIR} ${ARGN})

    target_link_libraries(${TARGET}
        PRIVATE
        Qt5::Widgets
        glbinding::glbinding
    )

    custom_add_sanitizers(${TARGET})
endfunction()

custom_add_example(Sample05 "Sample05")
target_link_libraries(Sample05 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)

custom_add_example(Sample06 "Sample06")
custom_distribute_assets("Sample06" "res06" Sample06)
target_link_libraries(Sample06 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)

custom_add_example(Sample07 "Sample07")
custom_distribute_assets("Sample07" "res07" Sample07)
target_link_libraries(Sample07 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)

custom_add_example(Sample08 "Sample08")
custom_distribute_assets("Sample08" "res08" Sample08)
target_link_libraries(Sample08 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)


custom_add_example(Sample09 "Sample09")
custom_distribute_assets("Sample09" "res09" Sample09)
target_link_libraries(Sample09 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)

custom_add_example(Sample10 "Sample10")
custom_distribute_assets("Sample10/res" "res10" Sample10)
target_link_libraries(Sample10 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform
    libs::tinyxml2)

custom_add_example(Sample12 "Sample12")
custom_distribute_assets("Sample12/res" "res12" Sample12)
target_link_libraries(Sample12 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)

custom_add_example(Sample13 "Sample13")
custom_distribute_assets("Sample13/res" "res13" Sample13)
target_link_libraries(Sample13 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)

custom_add_example(Sample14 "Sample14")
custom_distribute_assets("Sample14/res" "res14" Sample14)
target_link_libraries(Sample14 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)

custom_add_example(Sample15 "Sample15")
custom_distribute_assets("Sample15/res" "res15" Sample15)
target_link_libraries(Sample15 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform)

custom_add_example(Sample16 "Sample16")
custom_distribute_assets("Sample16/res" "res16" Sample16)
target_include_directories(Sample16 PRIVATE ${PCL_INCLUDE_DIRS})
target_link_libraries(Sample16 PRIVATE
    libs::libglcore
    libs::libmath
    libs::libplatform
    Eigen3::Eigen
    ${PCL_LIBRARIES})
