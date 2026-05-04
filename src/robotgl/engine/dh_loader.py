"""Modified DH (MDH) JSON loader for robotgl."""

import json
from pathlib import Path
from typing import Optional

from roboticstoolbox import DHRobot, RevoluteMDH, PrismaticMDH


class DHLinkDef:
    """Single MDH link definition from JSON."""

    def __init__(
        self,
        alpha: float,
        a: float,
        theta: float,
        d: float,
        joint_type: str = "revolute",
        joint_limits: Optional[list[float]] = None,
        mass: Optional[float] = None,
        mesh_paths: Optional[list[str]] = None,
        name: Optional[str] = None,
    ) -> None:
        self.alpha = alpha
        self.a = a
        self.theta = theta
        self.d = d
        self.joint_type = joint_type
        self.joint_limits = joint_limits or [-float("inf"), float("inf")]
        self.mass = mass
        self.mesh_paths = mesh_paths or []
        self.name = name


class RobotDef:
    """Complete robot definition from MDH JSON."""

    def __init__(
        self,
        name: str,
        links: list[DHLinkDef],
        tool: Optional[list] = None,
    ) -> None:
        self.name = name
        self.links = links
        self.tool = tool


class DHLoader:
    """Loads MDH robot definitions from JSON files."""

    @staticmethod
    def load(filepath: str | Path) -> RobotDef:
        """Load robot definition from MDH JSON file.

        Args:
            filepath: Path to MDH JSON file.

        Returns:
            RobotDef with parsed link definitions.
        """
        path = Path(filepath)
        with open(path, "r") as f:
            data = json.load(f)

        links = []
        for link_data in data.get("links", []):
            link = DHLinkDef(
                alpha=link_data["alpha"],
                a=link_data["a"],
                theta=link_data.get("theta", 0.0),
                d=link_data.get("d", 0.0),
                joint_type=link_data.get("joint_type", "revolute"),
                joint_limits=link_data.get("joint_limits"),
                mass=link_data.get("mass"),
                mesh_paths=link_data.get("mesh_paths"),
                name=link_data.get("name"),
            )
            links.append(link)

        return RobotDef(
            name=data.get("name", path.stem),
            links=links,
            tool=data.get("tool"),
        )

    @staticmethod
    def to_dh_robot(robot_def: RobotDef) -> DHRobot:
        """Convert RobotDef to roboticstoolbox DHRobot.

        Args:
            robot_def: Parsed robot definition.

        Returns:
            DHRobot instance using MDH convention.
        """
        links = []
        for link_def in robot_def.links:
            link_kwargs = {
                "alpha": link_def.alpha,
                "a": link_def.a,
                "d": link_def.d,
                "offset": link_def.theta,
                "name": link_def.name or "",
            }
            if link_def.joint_type == "prismatic":
                link_kwargs["qlim"] = link_def.joint_limits
                links.append(PrismaticMDH(**link_kwargs))
            else:
                link_kwargs["qlim"] = link_def.joint_limits
                links.append(RevoluteMDH(**link_kwargs))

        return DHRobot(links, name=robot_def.name, tool=robot_def.tool)
