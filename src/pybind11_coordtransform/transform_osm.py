from __future__ import annotations

import argparse
import os

import osmium as o

from . import gcj02towgs84, wgs84togcj02


class Handler(o.SimpleHandler):
    def __init__(
        self,
        *,
        mode: str,
        writer: o.SimpleWriter,
    ):
        super().__init__()
        modes = ["gcj02_to_wgs84", "wgs84_to_gcj02"]
        assert mode in modes, f"invalid mode={mode}, should be one of {modes}"
        self.translate_fn = gcj02towgs84 if mode == modes[0] else wgs84togcj02
        self.writer = writer

    def node(self, o):
        self.writer.add_node(self.update_node(o))

    def way(self, o):
        self.writer.add_way(o)

    def relation(self, o):
        self.writer.add_relation(o)

    def update_node(self, o):
        loc = o.location
        lon, lat = self.translate_fn(loc.lon, loc.lat)
        return o.replace(location=[lon, lat])


if __name__ == "__main__":
    prog = "python3 -m pybind11_coordtransform transform_osm"
    description = "transform osm file"
    parser = argparse.ArgumentParser(prog=prog, description=description)
    parser.add_argument(
        "input",
        type=str,
        help="input osm file",
    )
    parser.add_argument(
        "output",
        type=str,
        help="output osm file",
    )
    parser.add_argument(
        "--mode",
        type=str,
        help="should be 'gcj02_to_wgs84' or 'wgs84_to_gcj02'",
    )
    args = parser.parse_args()
    ipath = args.input
    opath = args.output
    mode = args.mode
    args = None

    os.makedirs(os.path.dirname(os.path.abspath(opath)), exist_ok=True)
    writer = o.SimpleWriter(opath)
    handler = Handler(mode=mode, writer=writer)
    handler.apply_file(ipath)
    writer.close()