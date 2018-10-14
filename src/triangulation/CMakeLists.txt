SET(sources
 triangulation.hpp
 triangulation.cpp
 delaunator.hpp delaunay.h edge.h numeric.h triangle.h vector2.h)

target_relative_sources(${CGRA_PROJECT} ${sources})
