# Results

Comparison of the fixed-gain PID baseline against the PPO adaptive gain scheduler (proposed) across all three gravity environments and motion tasks. Each pair of images uses the same seed, rollout length, and MuJoCo model — only the controller differs.

## Earth — 9.81 m/s²

<table>
<tr>
  <th align="center">Baseline (Fixed Gains)</th>
  <th align="center">Proposed (PPO Adaptive)</th>
</tr>
<tr>
  <td align="center"><b>Pose Hold</b><br/><img src="baseline/earth/baseline_pose_earth.png" width="100%"/></td>
  <td align="center"><b>Pose Hold</b><br/><img src="proposed/earth/proposed_pose_earth.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Sinusoidal Trajectory</b><br/><img src="baseline/earth/baseline_sinusoidal_earth.png" width="100%"/></td>
  <td align="center"><b>Sinusoidal Trajectory</b><br/><img src="proposed/earth/proposed_sinusoidal_earth.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Point-to-Point Trajectory</b><br/><img src="baseline/earth/baseline_p2p_earth.png" width="100%"/></td>
  <td align="center"><b>Point-to-Point Trajectory</b><br/><img src="proposed/earth/proposed_p2p_earth.png" width="100%"/></td>
</tr>
</table>

## Kepler-452b-Inspired — 12.164 m/s²

This gravity value is used as a repeatable high-gravity stress benchmark; it is not an asserted measurement of Kepler-452b surface gravity.

<table>
<tr>
  <th align="center">Baseline (Fixed Gains)</th>
  <th align="center">Proposed (PPO Adaptive)</th>
</tr>
<tr>
  <td align="center"><b>Pose Hold</b><br/><img src="baseline/kepler/baseline_pose_kepler.png" width="100%"/></td>
  <td align="center"><b>Pose Hold</b><br/><img src="proposed/kepler/proposed_pose_kepler.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Sinusoidal Trajectory</b><br/><img src="baseline/kepler/baseline_sinusoidal_kepler.png" width="100%"/></td>
  <td align="center"><b>Sinusoidal Trajectory</b><br/><img src="proposed/kepler/proposed_sinusoidal_kepler.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Point-to-Point Trajectory</b><br/><img src="baseline/kepler/baseline_p2p_kepler.png" width="100%"/><br/></td>
  <td align="center"><b>Point-to-Point Trajectory</b><br/><img src="proposed/kepler/proposed_p2p_kepler.png" width="100%"/></td>
</tr>
</table>

## Moon — 1.625 m/s²

<table>
<tr>
  <th align="center">Baseline (Fixed Gains)</th>
  <th align="center">Proposed (PPO Adaptive)</th>
</tr>
<tr>
  <td align="center"><b>Pose Hold</b><br/><img src="baseline/moon/baseline_pose_moon.png" width="100%"/></td>
  <td align="center"><b>Pose Hold</b><br/><img src="proposed/moon/proposed_pose_moon.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Sinusoidal Trajectory</b><br/><img src="baseline/moon/baseline_sinusoidal_moon.png" width="100%"/></td>
  <td align="center"><b>Sinusoidal Trajectory</b><br/><img src="proposed/moon/proposed_sinusoidal_moon.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Point-to-Point Trajectory</b><br/><img src="baseline/moon/baseline_p2p_moon.png" width="100%"/></td>
  <td align="center"><b>Point-to-Point Trajectory</b><br/><img src="proposed/moon/proposed_p2p_moon.png" width="100%"/></td>
</tr>
</table>

## Training Curves

Return and task error over training for each policy variant. The mixed-task policy was trained for 100,000 environment timesteps using seed 0.

<table>
<tr>
  <td align="center"><b>Mixed Task</b><br/><img src="training/training_results_mixed.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Pose Hold</b><br/><img src="training/training_results_pose.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Sinusoidal Trajectory</b><br/><img src="training/training_results_sin.png" width="100%"/></td>
</tr>
<tr>
  <td align="center"><b>Point-to-Point Trajectory</b><br/><img src="training/training_results_p2p.png" width="100%"/></td>
</tr>
</table>
