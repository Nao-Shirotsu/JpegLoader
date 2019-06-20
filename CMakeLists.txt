cmake_minimum_required(VERSION 3.10)
project(SimpleJpegLoader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(jpeg_exif_info
	JpegLoader/src/JpegLoader.cpp
	JpegLoader/src/JpegTagField.cpp
	JpegLoader/src/JpegTagUtils.cpp
	JpegLoader/src/main.cpp
)