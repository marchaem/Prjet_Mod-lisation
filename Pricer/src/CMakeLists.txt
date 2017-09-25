add_library(Cpp
  SHARED
  AsianOption.cpp BasketOption.cpp BlackScholesModel.cpp  Hedge.cpp MonteCarlo.cpp Option.cpp PerformanceOption.cpp parser.cpp)

install (TARGETS Cpp
	 DESTINATION lib)

file(
	GLOB 
	headers
	*.hpp
)

install(FILES ${headers}
  DESTINATION include/${CMAKE_PROJECT_NAME})
