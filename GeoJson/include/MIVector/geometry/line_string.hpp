#pragma once

// MIVector
#include <MIVector/geometry/point.hpp>
// stl
#include <vector>

namespace MIVector {
	namespace geometry {

		template <typename T, template <typename...> class Cont = std::vector>
		struct line_string : Cont<point<T> >
		{
			using coordinate_type = T;
			using point_type = point<T>;
			using container_type = Cont<point_type>;
			using container_type::container_type;
		};

	} // namespace geometry
} // namespace MIVector
