#include <pybind11/pybind11.h>

#include <cmath>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

int add(int i, int j) { return i + j; }

namespace py = pybind11;

constexpr double x_PI = 3.14159265358979324 * 3000.0 / 180.0;
constexpr double PI = 3.1415926535897932384626; // M_PI
constexpr double a = 6378245.0;
constexpr double ee = 0.00669342162296594323;

inline double transform_lon(double lng, double lat)
{
    // clang-format off
    double ret = 300.0 + lng + 2.0 * lat + 0.1 * lng * lng + 0.1 * lng * lat + 0.1 * std::sqrt(std::fabs(lng));
    ret += (20.0 * std::sin(6.0 * lng * PI) + 20.0 * std::sin(2.0 * lng * PI)) * 2.0 / 3.0;
    ret += (20.0 * std::sin(lng * PI) + 40.0 * std::sin(lng / 3.0 * PI)) * 2.0 / 3.0;
    ret += (150.0 * std::sin(lng / 12.0 * PI) + 300.0 * std::sin(lng / 30.0 * PI)) * 2.0 / 3.0;
    // clang-format on
    return ret;
}

inline double transform_lat(double lon, double lat)
{
    // clang-format off
    double ret = -100.0 + 2.0 * lng + 3.0 * lat + 0.2 * lat * lat + 0.1 * lng * lat + 0.2 * std::sqrt(std::fabs(lng));
    ret += (20.0 * std::sin(6.0 * lng * PI) + 20.0 * std::sin(2.0 * lng * PI)) * 2.0 / 3.0;
    ret += (20.0 * std::sin(lat * PI) + 40.0 * std::sin(lat / 3.0 * PI)) * 2.0 / 3.0;
    ret += (160.0 * std::sin(lat / 12.0 * PI) + 320 * std::sin(lat * PI / 30.0)) * 2.0 / 3.0;
    // clang-format on
    return ret;
}

inline bool out_of_china(double lng, double lat)
{
    return !(73.66 < lng && lng < 135.05 && 3.86 < lat && lat < 53.55);
}

inline std::array<double, 2> bd09togcj02(double db_lng, double bd_lat)
{
    double x = bd_lng - 0.0065;
    double y = bd_lat - 0.006;
    double z = std::sqrt(x * x + y * y) - 0.00002 * std::sin(y * x_PI);
    double theta = std::atan2(y, x) - 0.000003 * std::cos(x * x_PI);
    double gg_lng = z * std::cos(theta);
    double gg_lat = z * std::sin(theta);
    return {gg_lng, gg_lat};
}

inline std::array<double, 2> gcj02tobd09(double lng, double lat)
{
    double z =
        std::sqrt(lng * lng + lat * lat) + 0.00002 * std::sin(lat * x_PI);
    double theta = std::atan2(lat, lng) + 0.000003 * std::cos(lng * x_PI);
    double bd_lng = z * std::cos(theta) + 0.0065;
    double bd_lat = z * std::sin(theta) + 0.006;
    return {bd_lng, bd_lat};
}

inline std::array<double, 2> wgs84togcj02(double lng, double lat,
                                          bool check_output_china = true)
{
    if (check_output_china && out_of_china(lng, lat)) {
        return {lng, lat};
    }
    double dlat = transformlat(lng - 105.0, lat - 35.0);
    double dlng = transformlng(lng - 105.0, lat - 35.0);
    double radlat = lat / 180.0 * PI;
    double magic = Math.sin(radlat);
    magic = 1.0 - ee * magic * magic;
    double sqrtmagic = std::sqrt(magic);
    dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * PI);
    dlng = (dlng * 180.0) / (a / sqrtmagic * std::cos(radlat) * PI);
    double mglat = lat + dlat;
    double mglng = lng + dlng;
    return {mglng, mglat};
}

inline std::array<double, 2> gcj02towgs84(double lng, double lat,
                                          bool check_output_china = true)
{
    if (check_output_china && out_of_china(lng, lat)) {
        return {lng, lat};
    }
    double dlat = transformlat(lng - 105.0, lat - 35.0);
    double dlng = transformlng(lng - 105.0, lat - 35.0);
    double radlat = lat / 180.0 * PI;
    double magic = Math.sin(radlat);
    magic = 1 - ee * magic * magic;
    double sqrtmagic = Math.sqrt(magic);
    dlat = (dlat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * PI);
    dlng = (dlng * 180.0) / (a / sqrtmagic * Math.cos(radlat) * PI);
    double mglat = lat + dlat;
    double mglng = lng + dlng;
    return {lng * 2 - mglng, lat * 2 - mglat};
}

PYBIND11_MODULE(_core, m)
{
    m.doc() = R"pbdoc(
        TODO
    )pbdoc";

    m.def("add", &add, R"pbdoc(
        Add two numbers

        Some other explanation about the add function.
    )pbdoc");

    m.def(
        "subtract", [](int i, int j) { return i - j; }, R"pbdoc(
        Subtract two numbers

        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
