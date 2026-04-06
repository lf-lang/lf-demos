#!/usr/bin/env python3

import sys
import math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


def clamp(x, lo, hi):
    return lo if x < lo else hi if x > hi else x


def rc_norm(x, mid=1500, span=500):
    return clamp((x - mid) / span, -1.0, 1.0)


def infer_dt(steps):
    if len(steps) < 2:
        return 0.02
    diffs = np.diff(steps)
    mean_step = np.mean(diffs)
    return 0.02 * mean_step


def integrate(df):
    steps = df["step"].astype(float).to_numpy()
    roll = df["roll"].astype(float).to_numpy()
    pitch = df["pitch"].astype(float).to_numpy()
    yaw = df["yaw"].astype(float).to_numpy()

    dt = infer_dt(steps)

    deadband = 12
    mid = 1500.0

    roll[np.abs(roll - mid) < deadband] = mid
    pitch[np.abs(pitch - mid) < deadband] = mid
    yaw[np.abs(yaw - mid) < deadband] = mid

    roll_n = np.array([rc_norm(v) for v in roll])
    pitch_n = np.array([rc_norm(v) for v in pitch])
    yaw_n = np.array([rc_norm(v) for v in yaw])

    vmax = 1.2
    yawrate = math.radians(100)

    v_fwd = pitch_n * vmax
    v_right = roll_n * vmax
    yawrate_cmd = yaw_n * yawrate

    n = len(df)
    x = np.zeros(n)
    y = np.zeros(n)
    psi = np.zeros(n)

    for i in range(1, n):
        psi[i] = psi[i - 1] + yawrate_cmd[i - 1] * dt

        c = math.cos(psi[i - 1])
        s = math.sin(psi[i - 1])

        vx = c * v_fwd[i - 1] - s * v_right[i - 1]
        vy = s * v_fwd[i - 1] + c * v_right[i - 1]

        x[i] = x[i - 1] + vx * dt
        y[i] = y[i - 1] + vy * dt

    return x, y, psi


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 plot_drone_path.py rc_out.csv")
        sys.exit(1)

    csv_file = Path(sys.argv[1])
    if not csv_file.exists():
        print("CSV file not found.")
        sys.exit(1)

    df = pd.read_csv(csv_file)

    required = ["step", "roll", "pitch", "yaw", "throttle", "aux1", "aux2"]
    for col in required:
        if col not in df.columns:
            print(f"Missing column: {col}")
            sys.exit(1)

    x, y, psi = integrate(df)

    base_name = csv_file.stem

    # ---- Path Plot ----
    plt.figure()
    plt.plot(x, y)
    plt.scatter([x[0]], [y[0]], marker="o", label="start")
    plt.scatter([x[-1]], [y[-1]], marker="x", label="end")
    plt.axis("equal")
    plt.xlabel("x (m)")
    plt.ylabel("y (m)")
    plt.title("Drone Movement Map")
    plt.legend()
    plt.savefig(f"src/{base_name}_path.pdf", bbox_inches="tight")

    # ---- Yaw Plot ----
    plt.figure()
    plt.plot(df["step"], np.degrees(psi))
    plt.xlabel("step")
    plt.ylabel("yaw (deg)")
    plt.title("Yaw Over Time")
    plt.savefig(f"src/{base_name}_yaw.pdf", bbox_inches="tight")

    print(f"Saved: src/{base_name}_path.pdf")
    print(f"Saved: src/{base_name}_yaw.pdf")

    plt.show()


if __name__ == "__main__":
    main()
