# import serial
# ser = serial.Serial('/dev/ttyUSB0')  # open serial port
# print(ser.name)         # check which port was really used
# ser.write(b'hello')     # write a string
# ser.close()

from pythonping import ping
from time import sleep
#ip = "169.254.233.159"

#ping("169.254.255.255", verbose=True, timeout= 0.001)

file = open("pingRes.txt", "w")
count = 0
for i in range(0, 256):
    for j in range (0, 256):
        if (count % 1024) == 0:
            sleep(3)
            # j+= 1

        if (i == 0 and j == 0) or (i == 255 and j == 255): 
            continue

        ip = f"192.168.3.{j}"
        file.write(f"{ip}   ")
        ping(ip, verbose=True, count=1, timeout = 0.001, out = file)
        count += 1
        print(count)
file.close()