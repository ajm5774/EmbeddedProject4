cmake_minimum_required (VERSION 2.6)
project (CreateProcess)

include_directories(
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_BINARY_DIR}")
file(GLOB createprocess_src
	"*.h"
	"*.c"
)

add_executable(CreateProcess ${createprocess_src})
