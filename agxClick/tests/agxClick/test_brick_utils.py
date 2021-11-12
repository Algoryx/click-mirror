from utils.brick_utils import BrickUtils
import agx


class TestBrickUtils:

    def test_that_value_is_returned_when_exists(self):
        component = dict(val="a value")
        val = BrickUtils.get_component_attribute(component, "val")
        assert val == "a value"

    def test_that_default_is_returned_when_value_not_exists(self):
        component = dict()
        val = BrickUtils.get_component_attribute(component, "val", default="not found")
        assert val == "not found"

    def test_that_roll_pitch_yaw_is_calculated(self):
        quat = agx.Quat(0, 0, 0.922689838671033, 0.385543073615936)
        assert BrickUtils.roll_pitch_yaw(quat) == (0, 0, 2.35)
