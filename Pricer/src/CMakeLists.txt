add_library(Cpp
  SHARED
  *.cpp)

install (TARGETS Cpp
	 DESTINATION lib)

file(
	GLOB 
	headers
	*.hpp
)

install(FILES ${headers}
  DESTINATION include/${CMAKE_PROJECT_NAME})
