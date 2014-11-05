"""
Data model for the computation of the altimetry.

The input classes implement the `geo_interface`_  protocol
which enable them to be used directly with Shapely_ and allow
to easily load test data from a GeoJSON_ file.

.. _geo_interface: https://gist.github.com/sgillies/2217756
.. _Shapely: http://toblerity.org/shapely/manual.html#python-geo-interface
.. _GeoJSON: http://geojson.org/geojson-spec.html
"""

from itertools import chain
import json

from shapely import geometry
from shapely.validation import explain_validity


GEOMETRY_TYPES = (
    "Point", "MultiPoint",
    "LineString", "MultiLineString",
    "Polygon", "MultiPolygon",
)


def _preproc_one_coord(coord):
    if len(coord) not in (2, 3):
        raise ValueError("Coordinates are expected to be seq of 2 or 3 numbers")
    return tuple(float(c) for c in coord)

def _preproc_point_seq(coordinates):
    return [_preproc_one_coord(c) for c in coordinates]


class InconsistentGeometricModel(Exception):

    def __init__(self, message,
                 ids=None, witness_point=None, **kwargs):
        super(InconsistentGeometricModel, self).__init__(message)
        self.message = message
        self.ids = list(ids) if ids else []
        self.witness_point = witness_point
        self.__dict__.update(kwargs)

    def __str__(self):
        return self.message.format(**self.__dict__)

class GroundMaterial(object):

    def __init__(self, id_):
        self.id = id_


MATERIAL_WATER = GroundMaterial("Water")
DEFAULT_MATERIAL = GroundMaterial("__default__")
HIDDEN_MATERIAL =  GroundMaterial("__hidden__")


class GeometricFeature(object):
    geometric_type = None # To be overridden by derived classes

    def __init__(self, coords_or_shape, id):
        if isinstance(coords_or_shape, geometry.base.BaseGeometry):
            self.set_shape(coords_or_shape)
        else:
            self._coords = _preproc_point_seq(coords_or_shape)
            self._shape = None # to cache the Shapely shape
        if id:
            assert isinstance(id, basestring), 'id "%r" is not a string' % id
        self.id = id

    def set_shape(self, shape):
        self._shape = shape
        if isinstance(shape, geometry.base.BaseMultipartGeometry):
            self._coords = [subshape.coords for subshape in shape.geoms]
        else:
            self._coords = shape.coords

    def build_coordinates(self):
        raise NotImplementedError

    def build_geometry(self):
        if self.geometric_type not in GEOMETRY_TYPES:
            return None
        else:
            d = {"type": self.geometric_type,
                 "coordinates": self.build_coordinates()}
            return d

    def build_properties(self):
        return {}

    def build_feature(self):
        d = {"type": "Feature",
             "geometry" : self.build_geometry(),
             "properties": self.build_properties()}
        if self.id is not None:
            d["id"] = self.id
        return d

    def build_shape(self):
        return geometry.shape(self)

    @property
    def as_geojson(self):
        return json.dumps(self.build_feature(), sort_keys=True)

    @property
    def __geo_interface__(self):
        return self.build_geometry()

    @property
    def as_feature(self):
        return self.build_feature()

    @property
    def as_shape(self):
        return geometry.asShape(self)

    @property
    def shape(self):
        if self._shape is None:
            self._shape = self.build_shape()
        return self._shape

    def ensure_ok(self):
        shape = self.shape # CAUTION Cache the shape
        if shape.is_valid:
            return
        else :
            raise InconsistentGeometricModel("Invalid shapely shape : {details}",
                                             details=explain_validity(shape),
                                             ids=[self.id])

def elementary_shapes(shape):
    if isinstance(shape, geometry.base.BaseMultipartGeometry):
        return shape.geoms
    else:
        return (shape,)


class TympanFeature(GeometricFeature):

    def __init__(self, coords, parent_site=None, **kwargs):
        super(TympanFeature, self).__init__(coords, **kwargs)
        self._parent_site = None
        if parent_site:
            self.parent_site = parent_site

    @property
    def parent_site(self):
        """Parent site for this feature"""
        return self._parent_site

    @parent_site.setter
    def parent_site(self, parent):
        parent.add_child(self)
        self._parent_site = parent

    @parent_site.deleter
    def parent_site(self):
        self._parent_site.drop_child(self)
        self._parent_site = None

    @property
    def tympan_type(self):
        return type(self).__name__

    @property
    def parent_site_id(self):
        return  self.parent_site and self.parent_site.id

    def build_properties(self):
        p = super(TympanFeature, self).build_properties()
        assert "type" not in p
        p["type"] = self.tympan_type
        if self.parent_site:
            assert "site" not in p
            p["site"] = self.parent_site_id
        return p

    def __str__(self):
        return "%s #%s" % (self.__class__.__name__, self.id)


class LevelCurve(TympanFeature):
    geometric_type = "MultiLineString"

    def __init__(self, coords, altitude, close_it=False, **kwargs):
        self.altitude = float(altitude)
        if close_it:
            if isinstance(coords, geometry.base.BaseGeometry):
                raise ValueError("close_it=True is incompatible with passing an existing shape")
            else:
                coords = coords + [coords[0]] # Close the sequence of points
        super(LevelCurve, self).__init__(coords, **kwargs)

    def build_coordinates(self):
        return [self._coords]

    def build_properties(self):
        d = super(LevelCurve, self).build_properties()
        d.update(altitude=self.altitude)
        return d


class PolygonalTympanFeature(TympanFeature):
    geometric_type = "Polygon"

    def __init__(self, coords, **kwargs):
        self.holes = [_preproc_point_seq(hole) for hole in kwargs.pop("holes", [])]
        super(PolygonalTympanFeature, self).__init__(coords, **kwargs)

    def build_coordinates(self):
        return [self._coords] + self.holes

    def build_shape(self):
        shape = super(PolygonalTympanFeature, self).build_shape()
        if not shape.exterior.is_ccw:
            shape = geometry.polygon.orient(shape)
        return shape

    def ensure_ok(self):
        super(PolygonalTympanFeature, self).ensure_ok()
        if not self.shape.exterior.is_simple:
            raise InconsistentGeometricModel("Polygon is expected to be simple",
                                             ids=[self.id])


class MaterialArea(PolygonalTympanFeature):

    def __init__(self, coords, material, **kwargs):
        self.material = material
        super(MaterialArea, self).__init__(coords, **kwargs)

    @property
    def material_id(self):
        return self.material.id

    def build_properties(self):
        d = super(MaterialArea, self).build_properties()
        d.update(material=self.material.id)
        return d


class VegetationArea(MaterialArea):

    def __init__(self, coords, material, height, **kwargs):
        self.height = height
        super(VegetationArea, self).__init__(coords, material, **kwargs)


class WaterBody(MaterialArea, LevelCurve):
    geometric_type = "Polygon"

    def __init__(self, coords, **kwargs):
        super(WaterBody, self).__init__(coords, material=MATERIAL_WATER, **kwargs)


class SiteNode(PolygonalTympanFeature):

    CHILDREN_TYPES = ("LevelCurve", "MaterialArea", "VegetationArea", "WaterBody",
                      "SiteLandtake", "InfrastructureLandtake", "SiteNode")

    def __init__(self, coords, **kwargs):
        super(SiteNode, self).__init__(coords, **kwargs)

        self.children = {}
        for k in self.CHILDREN_TYPES:
            self.children[k] = []
        self.features_by_id = {}

    def add_child(self, child):
        self.children[child.tympan_type].append(child)
        self.features_by_id[child.id] = child
        assert len(self.children["SiteLandtake"]) <= 1, (
            "No more than one site landtake is allowed (%s already got %s)" %
            (self, self.children["SiteLandtake"]))

    def drop_child(self, child):
        """Remove a feature from site children"""
        del self.features_by_id[child.id]
        self.children[child.tympan_type].remove(child)

    @staticmethod
    def recursive_features_ids(site):
        if site is None:
            return []
        feature_ids = [feature.id for feature in site.all_features]
        for subsite in site.subsites:
                feature_ids.extend(SiteNode.recursive_features_ids(subsite))
        return feature_ids

    @property
    def level_curves(self):
        return self._iter_children("LevelCurve", "SiteLandtake", "WaterBody")

    @property
    def material_areas(self):
        return self._iter_children("MaterialArea", "VegetationArea",
                                   "WaterBody")

    @property
    def subsites(self):
        return self.children["SiteNode"][:]

    @property
    def site_landtake(self):
        return self.children["SiteLandtake"]

    @property
    def landtakes(self):
        return self.children["InfrastructureLandtake"][:]

    @property
    def all_features(self):
        """All child features but site nodes"""
        features_type = set(self.CHILDREN_TYPES) - set(["SiteNode"])
        return self._iter_children(*features_type)

    def _iter_children(self, *args):
        return chain(*[self.children[k] for k in args])

    @property
    def non_altimetric_features(self):
        return (self.children["MaterialArea"] +
                self.children["VegetationArea"] +
                self.children["InfrastructureLandtake"] )


class SiteLandtake(LevelCurve):
    """When the main site landtake is considered as a level curve"""


class InfrastructureLandtake(MaterialArea):

    def __init__(self, coords, **kwargs):
        super(InfrastructureLandtake, self).__init__(
            coords, material=HIDDEN_MATERIAL, **kwargs)
