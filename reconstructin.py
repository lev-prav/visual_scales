import numpy as np
import cv2 as cv 
import glob
import json

def read_config():
    with open("stereo_calib.json", "r") as conf:
        return json.load(conf)

imgL = cv.imread('test/cam_0_858_1683039813685_.tiff', cv.IMREAD_GRAYSCALE)
imgR = cv.imread('test/cam_1_858_1683039813685_.tiff', cv.IMREAD_GRAYSCALE)

calib_params = read_config()
print(calib_params)
'''
ректификация 
карта глубин 

reproject ot 3D 

'''

# stereo = cv.StereoBM_create(numDisparities=16, blockSize=15)
# disparity = stereo.compute(imgL,imgR)
# cv.imshow("disp", disparity)
# cv.waitKey()