"""

        c++/python version of https://github.com/wandergis/coordtransform

"""
from __future__ import annotations

import numpy

__all__ = [
    "bd09togcj02",
    "gcj02_to_wgs84_Nx2",
    "gcj02_to_wgs84_Nx3",
    "gcj02tobd09",
    "gcj02towgs84",
    "out_of_china",
    "wgs84_to_gcj02_Nx2",
    "wgs84_to_gcj02_Nx3",
    "wgs84togcj02",
]

def bd09togcj02(lng: float, lat: float) -> numpy.ndarray[numpy.float64[2, 1]]: ...
def gcj02_to_wgs84_Nx2(
    coords: numpy.ndarray[numpy.float64[m, 2]], check_out_of_china: bool = True
) -> numpy.ndarray[numpy.float64[m, 2]]: ...
def gcj02_to_wgs84_Nx3(
    coords: numpy.ndarray[numpy.float64[m, 3]], check_out_of_china: bool = True
) -> numpy.ndarray[numpy.float64[m, 3]]: ...
def gcj02tobd09(lng: float, lat: float) -> numpy.ndarray[numpy.float64[2, 1]]: ...
def gcj02towgs84(
    lng: float, lat: float, check_out_of_china: bool = True
) -> numpy.ndarray[numpy.float64[2, 1]]: ...
def out_of_china(lng: float, lat: float) -> bool: ...
def wgs84_to_gcj02_Nx2(
    coords: numpy.ndarray[numpy.float64[m, 2]], check_out_of_china: bool = True
) -> numpy.ndarray[numpy.float64[m, 2]]: ...
def wgs84_to_gcj02_Nx3(
    coords: numpy.ndarray[numpy.float64[m, 3]], check_out_of_china: bool = True
) -> numpy.ndarray[numpy.float64[m, 3]]: ...
def wgs84togcj02(
    lng: float, lat: float, check_out_of_china: bool = True
) -> numpy.ndarray[numpy.float64[2, 1]]: ...

__version__: str = "0.1.1"
