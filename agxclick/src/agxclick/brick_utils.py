# We cannot import Brick to top level because of bug --> Not possible to run outside agxViewer (debug) - fix on the way.
# from Brick.Physics import Component
import math
import agx
from typing import Tuple
import sys


class BrickUtils:

    @staticmethod
    def import_Brick():
        """
            Use:
            Brick = BrickUtils.import_Brick()
            Why:
            Because Brick can not be imported, and #import Brick actually takes quite some time (yet unknown - could be it's not cached)
            This convenience method speeds it up by forcing cache use.
        """
        assert "Brick" in sys.modules, "Brick was not available for import!"
        return sys.modules['Brick']

    @staticmethod
    def get_component_attribute(brick_component, attribute_name: str, default=None):
        """
            component - A Brick.Physics.Component to find attribute in
            attribute_name - the name of the attribute to find
            default - the value that should be returned if attribute_name does not exists in brick_component
        """
        # NOTE: There is unfortunately no way to ask a Brick.Physics.Component if it has an attribute, yet.
        try:
            return brick_component[attribute_name]
        except Exception:
            return default

    @staticmethod
    def roll_pitch_yaw(quat) -> Tuple[float, float, float]:
        """
            quat can be agx.Quat or Brick.Math.Quat
            returned tuple has order roll, pitch, yaw in radians
        """
#        from Brick.Math import Quat as BrickQuat
        # from
        # https://automaticaddison.com/how-to-convert-a-quaternion-into-euler-angles-in-python/
        if quat.__class__ == agx.Quat:
            x = quat.x()
            y = quat.y()
            z = quat.z()
            w = quat.w()
        else:  # Assume Brick.Math.Quat
            x = quat.X
            y = quat.Y
            z = quat.Z
            w = quat.W

        t0 = 2.0 * (w * x + y * z)
        t1 = 1.0 - 2.0 * (x * x + y * y)
        roll_rad = math.atan2(t0, t1)

        t2 = 2.0 * (w * y - z * x)
        t2 = 1.0 if t2 > +1.0 else t2
        t2 = -1.0 if t2 < -1.0 else t2
        pitch_rad = math.asin(t2)

        t3 = +2.0 * (w * z + x * y)
        t4 = +1.0 - 2.0 * (y * y + z * z)
        yaw_rad = math.atan2(t3, t4)

        return (roll_rad, pitch_rad, yaw_rad)
