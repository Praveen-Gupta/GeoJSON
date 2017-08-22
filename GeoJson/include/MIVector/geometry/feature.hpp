#pragma once

#include <MIVector/geometry/geometry.hpp>

#include <MIVector/variant.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

#include <optional>

namespace MIVector {
	namespace geometry {

		struct value;

		struct null_value_t
		{
			constexpr null_value_t() {}
			constexpr null_value_t(std::nullptr_t) {}
		};

		constexpr bool operator==(const null_value_t&, const null_value_t&) { return true; }
		constexpr bool operator!=(const null_value_t&, const null_value_t&) { return false; }
		constexpr bool operator<(const null_value_t&, const null_value_t&) { return false; }

		constexpr null_value_t null_value = null_value_t();

		// Multiple numeric types (uint64_t, int64_t, double) are present in order to support
		// the widest possible range of JSON numbers, which do not have a maximum range.
		// Implementations that produce `value`s should use that order for type preference,
		// using uint64_t for positive integers, int64_t for negative integers, and double
		// for non-integers and integers outside the range of 64 bits.
		using value_base = MIVector::util::variant<null_value_t, bool, uint64_t, int64_t, double, std::string,
			MIVector::util::recursive_wrapper<std::vector<value>>,
			MIVector::util::recursive_wrapper<std::unordered_map<std::string, value>>>;

		struct value : value_base
		{
			using value_base::value_base;

			///////////// Added by Praveen Gupta ///////////////////////////
			//value(std::initializer_list<value_base> il) : value_base (il) { }

			value(null_value_t i) : value_base(i) {}
			value(bool b) : value_base(b) {}
			value(uint64_t i) : value_base(i) {}
			value(int64_t i) : value_base(i) {}
			value(double i) : value_base(i) {}
			value(std::string i) : value_base(i) {}
			value(MIVector::util::recursive_wrapper<std::vector<value>> i) : value_base(i) {}
			value(MIVector::util::recursive_wrapper<std::unordered_map<std::string, value>> i) : value_base(i) {}
			value(MIVector::util::no_init i) : value_base(i) {}


		};

		using property_map = std::unordered_map<std::string, value>;

		// The same considerations and requirement for numeric types apply as for `value_base`.
		using identifier = MIVector::util::variant<uint64_t, int64_t, double, std::string>;

		template <class T>
		struct feature
		{
			using coordinate_type = T;
			using geometry_type = MIVector::geometry::geometry<T>; // Fully qualified to avoid GCC -fpermissive error.

			geometry_type geometry;
			property_map properties{};
			std::optional<identifier> id{};

			// GCC 4.9 does not support C++14 aggregates with non-static data member
			// initializers.
			feature(geometry_type geometry_,
				property_map properties_ = property_map{},
				std::optional<identifier> id_ = std::optional<identifier>{})
				: geometry(std::move(geometry_)),
				properties(std::move(properties_)),
				id(std::move(id_)) {}
		};

		template <class T>
		constexpr bool operator==(feature<T> const& lhs, feature<T> const& rhs)
		{
			return lhs.id == rhs.id && lhs.geometry == rhs.geometry && lhs.properties == rhs.properties;
		}

		template <class T>
		constexpr bool operator!=(feature<T> const& lhs, feature<T> const& rhs)
		{
			return !(lhs == rhs);
		}

		template <class T, template <typename...> class Cont = std::vector>
		struct feature_collection : Cont<feature<T>>
		{
			using coordinate_type = T;
			using feature_type = feature<T>;
			using container_type = Cont<feature_type>;
			using container_type::container_type;
		};

	} // namespace geometry
} // namespace MIVector
