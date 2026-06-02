set(target metric_accumulator_test)

add_executable(${target}
    tests/average_accumulator.cpp
    tests/categorical_accumulator.cpp
    tests/sum_average_accumulator.cpp
    ${CMAKE_SOURCE_DIR}/src/metric_accumulator.cpp
    ${CMAKE_SOURCE_DIR}/src/metric.cpp
    ${CMAKE_SOURCE_DIR}/src/metric_impl/code_lines_count.cpp
    ${CMAKE_SOURCE_DIR}/src/metric_impl/cyclomatic_complexity.cpp
    ${CMAKE_SOURCE_DIR}/src/metric_impl/parameters_count.cpp
)

target_link_libraries(${target}
    PRIVATE
        GTest::GTest
        GTest::Main
        metric_accumulator
        function
        file
)

file(GLOB test_files "${CMAKE_SOURCE_DIR}/src/metric_impl/tests/files/*.py")
foreach(file IN LISTS test_files)
    get_filename_component(file_name ${file} NAME)
    configure_file(${file} "${CMAKE_CURRENT_BINARY_DIR}/${file_name}" COPYONLY)
endforeach()

add_test(NAME ${target} COMMAND ${target})