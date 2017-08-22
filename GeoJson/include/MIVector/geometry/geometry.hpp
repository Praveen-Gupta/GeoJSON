#pragma once

#include <MIVector/geometry/point.hpp>
#include <MIVector/geometry/line_string.hpp>
#include <MIVector/geometry/polygon.hpp>
#include <MIVector/geometry/multi_point.hpp>
#include <MIVector/geometry/multi_line_string.hpp>
#include <MIVector/geometry/multi_polygon.hpp>

#include <MIVector/variant.hpp>

// stl
#include <vector>
#include <initializer_list>

namespace MIVector {
	namespace geometry {

		template <typename T, template <typename...> class Cont = std::vector>
		struct geometry_collection;

		template <typename T>
		using geometry_base = MIVector::util::variant<point<T>,
			line_string<T>,
			polygon<T>,
			multi_point<T>,
			multi_line_string<T>,
			multi_polygon<T>,
			geometry_collection<T>>;

		template <typename T>
		struct geometry : geometry_base<T>
		{
			using coordinate_type = T;
			using geometry_base<T>::geometry_base;
			///////////// Added by Praveen Gupta ///////////////////////////
			//geometry(std::initializer_list<geometry_base<T>> il) : geometry_base<T>(il) {}
			/*
			 * The default constructor would create a point geometry with default-constructed coordinates;
			 * i.e. (0, 0). Since this is not particularly useful, and could hide bugs, it is disabled.
			 */
			geometry() = delete;
			geometry(point<T> g) : geometry_base<T>(g) {}
			geometry(line_string<T> g) : geometry_base<T>(g) {}
			geometry(polygon<T> g) : geometry_base<T>(g) {}
			geometry(multi_point<T> g) : geometry_base<T>(g) {}
			geometry(multi_polygon<T> g) : geometry_base<T>(g) {}
			geometry(multi_line_string<T> g) : geometry_base<T>(g) {}
			geometry(geometry_collection<T> g) : geometry_base<T>(g) {}
		};

		template <typename T, template <typename...> class Cont>
		struct geometry_collection : Cont<geometry<T>>
		{
			using coordinate_type = T;
			using geometry_type = geometry<T>;
			using container_type = Cont<geometry_type>;

			geometry_collection() = default;
			geometry_collection(geometry_collection const&) = default;
			geometry_collection(geometry_collection &&) = default;
			geometry_collection(std::initializer_list<geometry_type> && args)
				: container_type(std::forward<std::initializer_list<geometry_type>>(args)) {};

			geometry_collection(std::initializer_list<geometry_type> args)
				: container_type(<std::initializer_list<geometry_type>>(args)) {};
		};

	} // namespace geometry
} // namespace MIVector
