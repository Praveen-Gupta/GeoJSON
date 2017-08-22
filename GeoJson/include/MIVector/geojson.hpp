#pragma once

#include <MIVector/geometry.hpp>
#include <MIVector/variant.hpp>

namespace MIVector {
	namespace geojson {

		using value = MIVector::geometry::value;
		using null_value_t = MIVector::geometry::null_value_t;
		using identifier = MIVector::geometry::identifier;
		using point = MIVector::geometry::point<double>;
		using multi_point = MIVector::geometry::multi_point<double>;
		using line_string = MIVector::geometry::line_string<double>;
		using linear_ring = MIVector::geometry::linear_ring<double>;
		using multi_line_string = MIVector::geometry::multi_line_string<double>;
		using polygon = MIVector::geometry::polygon<double>;
		using multi_polygon = MIVector::geometry::multi_polygon<double>;
		using geometry = MIVector::geometry::geometry<double>;
		using geometry_collection = MIVector::geometry::geometry_collection<double>;
		using feature = MIVector::geometry::feature<double>;
		using feature_collection = MIVector::geometry::feature_collection<double>;

		// Parse inputs of known types. Instantiations are provided for geometry, feature, and
		// feature_collection.
		template <class T>
		T parse(const std::string &);

		// Parse any GeoJSON type.
		using geojson = MIVector::util::variant<geometry, feature, feature_collection>;
		geojson parse(const std::string &);

		// Stringify inputs of known types. Instantiations are provided for geometry, feature, and
		// feature_collection.
		template <class T>
		std::string stringify(const T &);

		// Stringify any GeoJSON type.
		std::string stringify(const geojson &);

	} // namespace geojson
} // namespace MIVector
