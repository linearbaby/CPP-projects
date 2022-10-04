set(SOURCES car_db_functions.c)

add_library(car_low_level STATIC ${SOURCES})

target_link_libraries(car_low_level PRIVATE m)

target_include_directories(car_low_level
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        )
