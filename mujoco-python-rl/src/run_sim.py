import mujoco
import mujoco.viewer
import numpy as np
import csv
import time
import os

csv_path = "logs/trajectory.csv"
if not os.path.exists(csv_path):
    raise FileNotFoundError(csv_path)

with open(csv_path, "r") as f:
    reader = csv.reader(f)
    rows = list(reader)

header = rows[0]
data = np.array(rows[1:], dtype=float)

num_cols = data.shape[1]
num_q = num_cols
q_traj = data[:, :num_q]

model = mujoco.MjModel.from_xml_path("src/mujoco_env/3dof_leg.xml")
data_mj = mujoco.MjData(model)

mujoco.mj_resetData(model, data_mj)

with mujoco.viewer.launch_passive(model, data_mj) as viewer:
    for k in range(len(q_traj)):
        if not viewer.is_running():
            break

        data_mj.qpos[:num_q] = q_traj[k]
        mujoco.mj_forward(model, data_mj)

        viewer.sync()
        time.sleep(model.opt.timestep)
    print("sim done")
    while viewer.is_running():
        mujoco.mj_forward(model, data_mj)
        viewer.sync()
        time.sleep(model.opt.timestep)