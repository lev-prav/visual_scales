echo "run lidar"
lidar_start=lidar/rplidar_sdk/output/Linux/Release/ultra_simple

run_lidar=$'lidar_start --channel --tcp 192.168.0.7 20108'
run_tof=tof/bin/tof
 ./$run_tof &
