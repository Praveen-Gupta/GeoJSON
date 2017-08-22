# GeoJSON
## Provides header-only, generic C++ interfaces for geometry types, geometry collections, and features.

        geometry::point
        geometry::multi_point
        geometry::line_string
        geometry::multi_line_string
        geometry::polygon
        geometry::multi_polygon
        geometry::geometry_collection
        geometry::feature

## Design

These types are designed to be easy to parse and serialize to GeoJSON.

They should also be a robust and high performance container for data processing and conversion.
Goals

    Header-only
    Fast compile
    c++11/c++14 compatibility
    No external dependencies for usage of core types (point, line_string, etc)
