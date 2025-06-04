#!/bin/bash

roslaunch planning af_tracking.launch

python3 /root/Auto-Filmer/scripts/ros_logs.py
