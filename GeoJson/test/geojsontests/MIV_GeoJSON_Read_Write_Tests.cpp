#include "stdafx.h"
#include <MIVector/geojson.hpp>
#include <MIVector/geojson/rapidjson.hpp>
#include <MIVector/geometry.hpp>
#include <MIVector/geojson_impl.hpp>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iostream>
#include <windows.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MIVector::geojson;

class TestGlobals
{
public:
	static  std::string TestDataPath;
};

std::string TestGlobals::TestDataPath = "../../GeoJson/test/fixtures/";


template <class T>
std::string writeGeoJSON(const T& t, bool use_convert)
{
	if (use_convert)
	{
		rapidjson_allocator allocator;
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		convert(t, allocator).Accept(writer);
		return buffer.GetString();
	}
	else
	{
		return stringify(t);
	}
}

namespace MIV_GeoJSON_Tests
{
	TEST_CLASS(MIV_GeoJSON_Tests)
	{
	public:

		static geojson readGeoJSON(const std::string &path, bool use_convert)
		{
			std::ifstream t(path.c_str());
			std::stringstream buffer;
			buffer << t.rdbuf();
			if (use_convert)
			{
				rapidjson_document d;
				d.Parse<0>(buffer.str().c_str());
				return convert(d);
			}
			else
			{
				return parse(buffer.str());
			}
		}

		static void testPoint(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "point.json", use_convert);
			assert(data.is<geometry>());

			const auto &geom = data.get<geometry>();
			assert(geom.is<point>());

			const auto &p = geom.get<point>();
			assert(p.x == 30.5);
			assert(p.y == 50.5);

			std::string str = writeGeoJSON(data, use_convert);

			std::ofstream out(TestGlobals::TestDataPath + "output.json");
			out << str;
			out.close();
		}

		static void testMultiPoint(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "multi-point.json", use_convert);
			assert(data.is<geometry>());

			const auto &geom = data.get<geometry>();
			assert(geom.is<multi_point>());

			const auto &points = geom.get<multi_point>();
			assert(points.size() == 2);

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testLineString(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "line-string.json", use_convert);
			assert(data.is<geometry>());

			const auto &geom = data.get<geometry>();
			assert(geom.is<line_string>());

			const auto &points = geom.get<line_string>();
			assert(points.size() == 2);

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testMultiLineString(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "multi-line-string.json", use_convert);
			assert(data.is<geometry>());

			const auto &geom = data.get<geometry>();
			assert(geom.is<multi_line_string>());

			const auto &lines = geom.get<multi_line_string>();
			assert(lines.size() == 1);
			assert(lines[0].size() == 2);

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testPolygon(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "polygon.json", use_convert);
			assert(data.is<geometry>());

			const auto &geom = data.get<geometry>();
			assert(geom.is<polygon>());

			const auto &rings = geom.get<polygon>();
			assert(rings.size() == 1);
			assert(rings[0].size() == 5);
			assert(rings[0][0] == rings[0][4]);

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testMultiPolygon(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "multi-polygon.json", use_convert);
			assert(data.is<geometry>());

			const auto &geom = data.get<geometry>();
			assert(geom.is<multi_polygon>());

			const auto &polygons = geom.get<multi_polygon>();
			assert(polygons.size() == 1);
			assert(polygons[0].size() == 1);
			assert(polygons[0][0].size() == 5);
			assert(polygons[0][0][0] == polygons[0][0][4]);

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testGeometryCollection(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "geometry-collection.json", use_convert);
			assert(data.is<geometry>());

			const auto &geom = data.get<geometry>();
			assert(geom.is<geometry_collection>());

			const auto &collection = geom.get<geometry_collection>();
			assert(collection[0].is<point>());
			assert(collection[1].is<line_string>());

			assert(parse(writeGeoJSON(data, use_convert)) == data);

			std::string str = writeGeoJSON(data, use_convert);
			std::string outfile = TestGlobals::TestDataPath + "output.json";
			std::ofstream out(outfile);
			out << str;
			out.close();
		}

		static void testFeature(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "feature.json", use_convert);
			assert(data.is<feature>());

			const auto &f = data.get<feature>();
			assert(f.geometry.is<point>());

			assert(f.properties.at("bool").is<bool>());
			assert(f.properties.at("bool") == true);
			assert(f.properties.at("string").is<std::string>());
			assert(f.properties.at("string").get<std::string>() == "foo");
			assert(f.properties.at("double") == 2.5);
			assert(f.properties.at("double").is<double>());
			assert(f.properties.at("uint").get<std::uint64_t>() == 10);
			assert(f.properties.at("uint").is<std::uint64_t>());
			assert(f.properties.at("int").get<std::int64_t>() == -10);
			assert(f.properties.at("int").is<std::int64_t>());
			assert(f.properties.at("null").is<MIVector::geometry::null_value_t>());
			assert(f.properties.at("null") == MIVector::geometry::null_value_t{});

			using prop_map = std::unordered_map<std::string, value>;
			using values = std::vector<value>;

			const auto &nested = f.properties.at("nested");

			assert(nested.is<MIVector::util::recursive_wrapper<values>>());
			assert(nested.get<values>().at(0).is<std::uint64_t>());
			assert(nested.get<values>().at(0).get<std::uint64_t>() == 5);
			assert(nested.get<values>().at(1).is<MIVector::util::recursive_wrapper<prop_map>>());
			assert(nested.get<values>().at(1).get<prop_map>().at("foo").is<std::string>());
			assert(nested.get<values>().at(1).get<prop_map>().at("foo").get<std::string>() == "bar");

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testFeatureNullProperties(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "feature-null-properties.json", use_convert);
			assert(data.is<feature>());

			const auto &f = data.get<feature>();
			assert(f.geometry.is<point>());
			assert(f.properties.size() == 0);

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testFeatureMissingProperties(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "feature-missing-properties.json", use_convert);
			assert(data.is<feature>());

			const auto &f = data.get<feature>();
			assert(f.geometry.is<point>());
			assert(f.properties.size() == 0);

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testFeatureCollection(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "feature-collection.json", use_convert);
			assert(data.is<feature_collection>());

			const auto &features = data.get<feature_collection>();
			assert(features.size() == 2);

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testFeatureID(bool use_convert)
		{
			const auto &data = readGeoJSON(TestGlobals::TestDataPath + "feature-id.json", use_convert);
			assert(data.is<feature_collection>());

			const auto &features = data.get<feature_collection>();

			assert(features.at(0).id == identifier{ uint64_t(1234) });
			assert(features.at(1).id == identifier{ "abcd" });

			assert(parse(writeGeoJSON(data, use_convert)) == data);
		}

		static void testParseErrorHandling()
		{
			try
			{
				readGeoJSON(TestGlobals::TestDataPath + "invalid.json", false);
				assert(false && "Should have thrown an error");
			}
			catch (const std::runtime_error& err) {
				assert(std::string(err.what()).find("Invalid") != std::string::npos);
			}
		}

		TEST_METHOD(MIVTesTGeoJSON_ParseErrorHandling)
		{
			testParseErrorHandling();
		}

		TEST_METHOD(MIVTestGeoJSON_Point)
		{
			testPoint(true);
			testPoint(false);
		}

		TEST_METHOD(MIVTestGeoJSON_MultiPoint)
		{
			testMultiPoint(true);
			testMultiPoint(false);
		}

		TEST_METHOD(MIVTestGeoJSON_LineString)
		{
			testLineString(true);
			testLineString(false);
		}

		TEST_METHOD(MIVTestGeoJSON_MultiLineString)
		{
			testMultiLineString(true);
			testMultiLineString(false);
		}

		TEST_METHOD(MIVTestGeoJSON_Polygon)
		{
			testPolygon(true);
			testPolygon(false);
		}

		TEST_METHOD(MIVTestGeoJSON_MultiPolygon)
		{
			testMultiPolygon(true);
			testMultiPolygon(false);
		}

		TEST_METHOD(MIVTestGeoJSON_GeometryCollection)
		{
			testGeometryCollection(true);
			testGeometryCollection(false);
		}

		TEST_METHOD(MIVTestGeoJSON_Feature)
		{
			testFeature(true);
			testFeature(false);
		}

		TEST_METHOD(MIVTestGeoJSON_FeatureNullProperties)
		{
			testFeatureNullProperties(true);
			testFeatureNullProperties(false);
		}

		TEST_METHOD(MIVTestGeoJSON_FeatureMissingProperties)
		{
			testFeatureMissingProperties(true);
			testFeatureMissingProperties(false);
		}

		TEST_METHOD(MIVTestGeoJSON_FeatureCollection)
		{
			testFeatureCollection(true);
			testFeatureCollection(false);
		}

		TEST_METHOD(MIVTestGeoJSON_FeatureID)
		{
			testFeatureID(true);
			testFeatureID(false);
		}
	};
}

