from typing import List
from agxclick import ClickObject, ClickRobot, BrickUtils


def has_click_configuration(scene_positioninput) -> bool:
    return BrickUtils.get_component_attribute(scene_positioninput, "clickobjects") is not None


def get_click_configuration(click_scene) -> List[ClickRobot]:
    """
    click_scene must be a Brick ClickScene root
    Will only return objects defined in clickobjects entry of ClickScene
    """
    objects = []
    import Brick.Robotics
    for object in click_scene["clickobjects"]:
        if isinstance(object, Brick.Robotics.Robot):
            objects.append(ClickRobot(object))
        else:
            objects.append(ClickObject(object))
    return objects


def find_robots_in_scene(scene_positioninput) -> List[ClickRobot]:
    """
    Looks for Brick.Robotics.Robot:s in scene
    Will not look for ClickScene entry
    """
    from Brick.Physics import Component

    def is_robot(robot: Component):
        # There can be robots in urdf that is not actual robots, which is why we check #arms
        import Brick.Robotics
        return isinstance(robot, Brick.Robotics.Robot) and len(robot.Arms) > 0

    def find_robots(component: Component):
        if is_robot(component):
            return [component]
        else:
            brick_robots = list()
            for comp in component.InternalComponents:
                brick_robots.extend(find_robots(comp))
            return brick_robots

    brick_robots = find_robots(scene_positioninput)
    robots = []
    for brick_robot in brick_robots:
        robot = ClickRobot(brick_robot)
        robots.append(robot)

    return robots
