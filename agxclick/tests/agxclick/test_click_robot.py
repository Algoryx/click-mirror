from agxclick import ClickRobot


class FakeClickRobot(ClickRobot):
    def __init__(self):
        pass


class FakeJoint:
    def __init__(self, id: int):
        self.id = id
        self.ProtocolReference = id

    def __str__(self):
        return "1"

    def __eq__(self, rhs):
        return self.id == rhs.id


class FakeArm:
    Joints = [FakeJoint(1), FakeJoint(2)]


class FakeBrickRobot:
    Arms = [FakeArm(), FakeArm()]


def test_that_joints_returns_all_joints():
    robot = FakeClickRobot()
    robot.brickrobot = FakeBrickRobot()
    assert robot.joints() == [FakeJoint(1), FakeJoint(2), FakeJoint(1), FakeJoint(2)]


def test_that_joint_protocolrefs_returns_all_jointrefs():
    robot = FakeClickRobot()
    robot.brickrobot = FakeBrickRobot()
    assert robot.joint_protocolrefs() == [1, 2, 1, 2]
