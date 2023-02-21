import subprocess
import os

class IRunnable:
    def get_run_command(self):
        raise Exception("NOT IMPLEMENTED")
    
    def get_args(self) -> list:
        """
        The first arg is ALWAYS the path to executable 
        """
        raise Exception("NOT IMPLEMENTED")

class Lidar(IRunnable):
    path = ""
    ip = ""
    port = ""

    def __init__(self, path : str, ip : str, port : str, save_dir : str, time : str) -> None:
        self.path = path
        self.ip = ip 
        self.port = port
        self.save_dir = save_dir
        self.time = time
    
    def get_run_command(self):
       return f"{self.path} --channel --tcp {self.ip} {self.port} {self.save_dir} {self.time}"
    
    def get_args(self) -> list:
        return [self.path, "--channel", "--tcp", self.ip,self.port, self.save_dir, self.time]

class Tof(IRunnable):
    path = ""
    
    def __init__(self, path : str, time_mls : str) -> None:
        self.path = path
        self.time = time_mls
        self.redirect_output = " > data/tof_log.txt"

    def get_run_command(self):
        return f"{self.path} {self.time} "
    
    def get_args(self) -> list:
        return [self.path, self.time, self.redirect_output]
    
class Camera(IRunnable):
    path = ""
    save_dir = ""

    def __init__(self, path: str, save_dir: str) -> None:
        self.path = path 
        self.save_dir = save_dir

    def get_run_command(self):
        return f"{self.path} {self.save_dir}"
    def get_args(self) -> list:
        return [self.path, self.save_dir]

timer = 10000 # эту штуку можно менять. ОСТАЛЬНЫЕ НЕ НАДО 

lidar = Lidar(
    "lidar/rplidar_sdk/output/Linux/Release/ultra_simple", 
    "192.168.0.7",
    "20108",
    "/home/lev/data/4Cameras/visual_scales/data",
    timer
    )
tof = Tof("tof/bin/tof", timer)
basler = Camera("pylon_example/bin/pylon_example", "/home/lev/data/4Cameras/visual_scales/data")

devices = [lidar, tof, basler]
run_commands = []

for device in devices:
    run_commands.append(device.get_run_command())
    #print(run_command)
    #os.system(f" {run_command}")
    #subprocess.run(device.get_args()) 

run = " & \n".join(run_commands) + " & \n"
print(run)

os.system(run)
