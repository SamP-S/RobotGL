import numpy as np
import time
import threading
import roboticstoolbox as rtb
from spatialmath import SE3

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
            if (line[0] == '#'):
                continue
            parts = [p.strip() for p in line.strip().split(',') if p.strip() != '']
            if len(parts) < 5:
                raise ValueError(f"Invalid DH parameter line (expect 5 cols): {line!r}")
            name = parts[0]
            try:
                active = parts[1].lower() == 'true'
                d = float(parts[2])
                theta = float(parts[3])
                a = float(parts[4])
                alpha = float(parts[5])
                qlim_low = float(parts[6])
                qlim_high = float(parts[7])
            except ValueError:
                raise ValueError(f"Non-numeric DH values in line: {line!r}")
            dh.append((name, active, a, alpha, d, theta, qlim_low, qlim_high))
    return dh

def build_robot(dh_params):
    """Build an rtb.DHRobot from DH parameter rows.

    dh_params: sequence of (name, active, a, alpha, d, theta) tuples.
    Each link is created with its name, and if 'active' is False, the link is marked as dummy.
    """
    links = []
    for i, (name, active, a, alpha, d, theta, qlim_low, qlim_high) in enumerate(dh_params):
        # Use RevoluteDH; offset sets the constant theta term
        link = None
        if active:
            link = rtb.RevoluteDH(d=d, a=a, alpha=alpha, offset=theta, qlim=[qlim_low, qlim_high])
        else:
            link = rtb.RevoluteDH(d=d, a=a, alpha=alpha, offset=theta, qlim=[0, 0])  # dummy link
        links.append(link)
    robot = rtb.DHRobot(links, name="robot_arm")
    return robot

def generate_circle_points(centre, radius, num_points=31):
    """Generate 3D points that form a circle in the XY plane at given Z height."""
    points = []
    for i in range(num_points):
        angle = 2 * np.pi * i / (num_points - 1)
        # x = centre[0] + radius * np.cos(angle)
        # y = centre[1] + radius * np.sin(angle)
        # z = centre[2]
        x = centre[0] + radius * np.cos(angle)
        y = centre[1] + radius * np.sin(angle)
        z = centre[2]
        points.append((x, y, z))
    return points

def generate_poses(robot, target_points):
    """Generate joint-space poses that move the end-effector in a circle.

    Returns (poses, points) where poses is a list of joint-angle lists
    (length = number_of_joints) and points are the corresponding end-effector
    world positions.
    """
    poses = []
    points = []
    n_joints = robot.n

    # calc home pose (all zeros)
    home = [0.0] * n_joints
    poses.append(list(home))
    fk = robot.fkine(home)
    points.append(tuple(fk.t))

    # Use position-only IK (mask) because the arm may be 5-DOF
    mask = [1, 1, 1, 0, 0, 0]

    for i in range(1, len(target_points)):
        target = target_points[i]
        T = SE3(target[0], target[1], target[2])
        try:
            sol = robot.ikine_LM(T, q0=home, mask=mask)
        except Exception as e:
            # if IK fails, append the home pose as a fallback
            poses.append(list(home))
            points.append(tuple(robot.fkine(home).t))
            continue
        q = list(sol.q)
        poses.append(q)
        fk = robot.fkine(q)
        points.append(tuple(fk.t))

    return poses, points


def main():
    # opengl visualiser
    vis = Visualiser()
    vis.createWindow()
    
    # open dual arm controller
    dctrl = DualArmController(port_base="/dev/ttyACM0", port_forearm="/dev/ttyUSB0")
    
    # build kinematics
    dh_params = load_dh_parameters("/home/trio/source/RobotGL/tools/v2/python/dh_raw.txt")
    robot = build_robot(dh_params)
    target_points = generate_circle_points(
        #centre=(-0.1608, 0.0, 0.4161),
        centre=(-0.14, 0.14, 0.37),
        radius=0.05,
        num_points=31
    )
    poses, pose_points = generate_poses(
        robot,
        target_points
    )
    
    for i in range(1, len(poses)):
        dctrl.enqueue_set_pose(i, poses[i])
    for i in range(1, len(poses)):
        time.sleep(0.05)
        dctrl.tick()
    for i in range(1, len(poses)):
        dctrl.enqueue_goto(i)
    
    index = 0
    start_time = vis.getTicks()
    elapsed_time = 0
    while True:
        # queue motions if nothing in pending
        if dctrl.getQueueSizes() == (0, 0):
            if input("Enter to continue...") == "reset":
                dctrl.enqueue_goto(0)
            else:
                for i in range(1, len(poses)):
                    dctrl.enqueue_goto(i)
                    
        # read in responses and send commands
        dctrl.tick()

        vis.begin_frame()
        vis.clear()
        vis.drawOrigin(size=0.05)
        
        current_time = vis.getTicks()
        elapsed_time = current_time - start_time
        # index = elapsed_time // 250
        index = dctrl.getCurrentPoses()[0]
        
        # draw target and actual points
        if True:
            # target positions
            if elapsed_time % 2000 < 1000:
                for point in target_points:
                    vis.drawPoint(point, colour=(1,1,0))
            # real positions
            else:
                for point in pose_points:
                    vis.drawPoint(point, colour=(0,1,1))
                    
        # draw robot at current pose
        if True:
            vis.drawRobot(robot, q=poses[index % len(poses)], size=0.03)
        
        # draw test poses
        if True:
            angle = 0.0 * (np.pi / 180.0)
            q = [angle] * robot.n
            q[0] = 0.0 * (np.pi / 180.0)
            q[1] = 45.0 * (np.pi / 180.0)
            q[2] = -45.0 * (np.pi / 180.0)
            q[3] = 0.0 * (np.pi / 180.0)
            q[4] = 0.0 * (np.pi / 180.0)
            fkin = robot.fkine(q)
            if (index == 0):
                print(f"q={q} -> T=\n{fkin}")
            for joint_idx, dh in enumerate(dh_params):
                name, active, a, alpha, d, theta, qlim_low, qlim_high = dh
                if active:
                    q[joint_idx] = 0.0
                    break
    
            vis.drawRobot(robot, q=q, size=0.03)
        
        vis.end_frame()
        time.sleep(0.1)
        
        

if __name__ == "__main__":
    main()
