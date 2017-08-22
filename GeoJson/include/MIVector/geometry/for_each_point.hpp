#pragma once

#include <MIVector/geometry/geometry.hpp>

namespace MIVector {
	namespace geometry {

		template <typename Point, typename F>
		auto for_each_point(Point&& point, F&& f)
			-> decltype(point.x, point.y, void())
		{
			f(std::forward<Point>(point));
		}

		template <typename Container, typename F>
		auto for_each_point(Container&& container, F&& f)
			-> decltype(container.begin(), container.end(), void());

		template <typename...Types, typename F>
		void for_each_point(MIVector::util::variant<Types...> const& geom, F&& f)
		{
			MIVector::util::variant<Types...>::visit(geom, [&](auto const& g) {
				for_each_point(g, f);
			});
		}

		template <typename...Types, typename F>
		void for_each_point(MIVector::util::variant<Types...> & geom, F&& f)
		{
			MIVector::util::variant<Types...>::visit(geom, [&](auto & g) {
				for_each_point(g, f);
			});
		}

		template <typename Container, typename F>
		auto for_each_point(Container&& container, F&& f)
			-> decltype(container.begin(), container.end(), void())
		{
			for (auto& e : container) {
				for_each_point(e, f);
			}
		}

	} // namespace geometry
} // namespace MIVector
