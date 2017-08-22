#pragma once

#include <MIVector/geometry/point.hpp>

namespace MIVector {
	namespace geometry {

		template <typename T>
		struct box
		{
			using point_type = point<T>;

			constexpr box(point_type const& min_, point_type const& max_)
				: m_min(min_), m_max(max_)
			{}

			point_type m_min;
			point_type m_max;
		};

		template <typename T>
		constexpr bool operator==(box<T> const& lhs, box<T> const& rhs)
		{
			return lhs.m_min == rhs.m_min && lhs.m_max == rhs.m_max;
		}

		template <typename T>
		constexpr bool operator!=(box<T> const& lhs, box<T> const& rhs)
		{
			return lhs.m_min != rhs.m_min || lhs.m_max != rhs.m_max;
		}

	} // namespace geometry
} // namespace MIVector
