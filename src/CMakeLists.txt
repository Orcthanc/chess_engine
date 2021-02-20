project( Chess )

file( GLOB_RECURSE ${PROJECT_NAME}_SOURCES "*/*.cpp" )

add_executable( ${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} "main.cpp" )

target_include_directories( ${PROJECT_NAME} PUBLIC "." )
