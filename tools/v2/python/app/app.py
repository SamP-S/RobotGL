import numpy as np
import time
import threading
from ikpy.chain import Chain
from ikpy.link import OriginLink, URDFLink

from visualiser import Visualiser
from dual_arm_controller import DualArmController

def load_dh_parameters(filename="dh_raw.txt"):
    """Load DH parameters from CSV file with header.

    Expected CSV format (header then rows): name,a,alpha,d,theta
    Returns a list of tuples (name, a, alpha, d, theta).
    """
    dh = []
    with open(filename, "r") as f:
        next(f)  # skip header
        for line in f:
            parts = [p.strip() for p in line.strip().split(',') if p.strip() != '']
            if len(parts) < 5:
                raise ValueError(f"Invalid DH parameter line (expect 5 cols): {line!r}")
            name = parts[0]
            try:
                a = float(parts[1])
                alpha = float(parts[2])
                d = float(parts[3])
                theta = float(parts[4])
            except ValueError:
                raise ValueError(f"Non-numeric DH values in line: {line!r}")
            dh.append((name, a, alpha, d, theta))
    return dh

def build_chain(dh_params):
    """Build an ikpy Chain using URDFLink constructed from DH-style rows.

    dh_params: sequence of (name, a, alpha, d, theta) tuples.
    We map DH parameters to URDFLink origin and rotation fields. This keeps
    compatibility with ikpy versions where DHLink is limited.
    """
    links = [OriginLink()]
    for i, (name, a, alpha, d, theta) in enumerate(dh_params):
        link_name = name if name else f"joint_{i}"
        links.append(
            URDFLink(
                name=link_name,
                origin_translation=[a, d, 0],
                origin_orientation=[alpha, 0, 0],
                rotation=[0, 0, theta]
            )
        )
    active_links = [False] + [True] * (len(links) - 1)  # all except OriginLink
    return Chain(name="robot_arm_chain", links=links, active_links_mask=active_links)

def generate_circle_points(centre, radius, num_points=31):
    """Generate 3D points that form a circle in the XY plane at given Z height."""
    points = []
    for i in range(num_points):
        angle = 2 * np.pi * i / (num_points - 1)
        x = centre[0] + radius * np.cos(angle)
        y = centre[1] + radius * np.sin(angle)
        z = centre[2]
        points.append((x, y, z))
    return points

def generate_poses(chain, target_points):
    """Generate joint-space poses that move the end-effector in a circle.

    Returns (poses, points) where poses is a list of joint-angle lists
    (length = number_of_joints) and points are the corresponding end-effector
    world positions.
    """
    poses = []
    points = []
    n_joints = len(chain.links) - 1  # exclude OriginLink

    # calc home pose
    poses.append([0.0] * (n_joints+1))  # Pose 0 is home: all-zero joint angles
    fk_matrix = chain.forward_kinematics(poses[0])
    fk_orientation = fk_matrix[:3, :3]
    fk_position = fk_matrix[:3, 3]
    points.append(tuple(fk_position))

    # calc other poses
    for i in range(1, len(target_points)):
        target = target_points[i]
        # inverse_kinematics returns a full vector including base element
        sol = chain.inverse_kinematics(target_position=target)
        # store only the joint angles (exclude the first element which is the base)
        joint_angles = list(sol[1: n_joints+1])
        poses.append(joint_angles)
        # compute FK using the full solution vector
        fk_matrix = chain.forward_kinematics(sol)
        fk_orientation = fk_matrix[:3, :3]
        fk_position = fk_matrix[:3, 3]
        points.append(tuple(fk_position))

    return poses, points


def main():
    # opengl visualiser
    vis = Visualiser()
    vis.createWindow()
    
    # build kinematics
    dh_params = load_dh_parameters("/home/trio/source/RobotGL/tools/v2/python/dh_raw.txt")
    chain = build_chain(dh_params)
    target_points = generate_circle_points(
        centre=(0.0015, 0.0015, 0.0015),
        radius=0.002,
        num_points=31
    )
    poses, points = generate_poses(
        chain,
        target_points
    )
    
    # print("Generated Poses:")
    # for i, pose in enumerate(poses):
    #     print(f"Pose {i}: {pose}")
    # print("\nGenerated Points:")
    # for i, point in enumerate(points):
    #     print(f"Point {i}: {point}")
    
    index = 0
    while True:
        vis.begin_frame()
        vis.clear()
        vis.drawOrigin(size=0.001)
        vis.drawPoint(points[0], colour=(1,0,0))
        vis.drawPoint(target_points[0], colour=(0,1,0))

        sub_target_points = target_points[:len(target_points)//2]
        sub_pose_points = points[len(points)//2:len(points)-1]
        for point in sub_target_points:
            vis.drawPoint(point, colour=(1,1,0))
        for point in sub_pose_points:
            vis.drawPoint(point, colour=(0,1,1))

        # vis.drawPose(poses[0], colour=(0,0,1))
        
        vis.end_frame()
        index = (index + 1) % len(poses)
        
    # TODO:
    # - integrate visualiser
    # - integrate dual arm controller


if __name__ == "__main__":
    main()
