set(SOURCES data_saver.c data_loader.c metric.c UI_functions.c)

add_library(car STATIC ${SOURCES})

target_link_libraries(car car_low_level)

target_include_directories(car
        INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        )
