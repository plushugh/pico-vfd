project(vfd)

add_library(vfd STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/src/vfd.c
)
target_link_libraries(vfd PUBLIC
	pico_stdlib
	hardware_spi
)

target_include_directories(vfd PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)
