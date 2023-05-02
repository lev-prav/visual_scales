import numpy as np
import cv2 as cv
import glob
import json


def calibrate(images_regex):
    col, row = 6, 5

    # termination criteria
    criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)
    # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
    objp = np.zeros((col*row,3), np.float32)
    objp[:,:2] = np.mgrid[0:row,0:col].T.reshape(-1,2)
    # Arrays to store object points and image points from all the images.
    objpoints = [] # 3d point in real world space
    imgpoints = [] # 2d points in image plane.
    images = glob.glob(images_regex)

    print(len(images))
    
    img = '' 
    gray = ''
    
    for fname in images:
        img = cv.imread(fname)
        gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
        # Find the chess board corners
        ret, corners = cv.findChessboardCorners(gray, (row,col), None)
        # If found, add object points, image points (after refining them)
        if ret == True:
            objpoints.append(objp)
            corners2 = cv.cornerSubPix(gray,corners, (11,11), (-1,-1), criteria)
            imgpoints.append(corners2)
            # Draw and display the corners
            # cv.drawChessboardCorners(img, (row,col), corners2, ret)
            # cv.imshow('img', img)
            # cv.waitKey(500)
        print(f"{fname} done")

        if len(objpoints) > 40:
            break

    cv.destroyAllWindows()

    print(len(objpoints), len(imgpoints))

    ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)
    h,  w = img.shape[:2]
    newcameramtx, roi = cv.getOptimalNewCameraMatrix(mtx, dist, (w,h), 1, (w,h))

    print("original ", mtx, dist)
    print("new matrix ", newcameramtx)

    return [mtx, dist, objpoints, imgpoints]
    # return {
    #     "original_matrix" : mtx.tolist(), 
    #     "original_dist" :   dist.tolist(), 
    #     "optimal_matrix" : newcameramtx.tolist()
    # }


tof =       ["tof", 'calibrate/tof_*.tiff']
cam_0 =     ["cam_0_right_from_tof",'test/cam_0_*.tiff']
cam_1 =     ["cam_1_left_from_tof", 'test/cam_1_*.tiff']

cam_0_mtx, cam_0_dist, cam_0_objpoint, cam_0_imgpoints = calibrate(cam_0[1])
cam_1_mtx, cam_1_dist, cam_1_objpoint, cam_1_imgpoints = calibrate(cam_1[1])
img_size = (1600,1200,)

retval, cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, R, T, E, F = cv.stereoCalibrate(
    cam_0_objpoint, 
    cam_0_imgpoints, 
    cam_1_imgpoints, 
    cam_0_mtx, 
    cam_0_dist, 
    cam_1_mtx, 
    cam_1_dist,
    img_size, 
)

'''
----------
[[797.40397903   0.         775.87727043]
 [  0.         800.31515916 621.11013703]
 [  0.           0.           1.        ]] [[-0.42961833  0.32245042 -0.00098452 -0.01198521 -0.18298627]] [[809.40322143   0.         770.6769373 ]
 [  0.         809.23186347 557.61098066]
 [  0.           0.           1.        ]] [[-3.58111390e-01  1.78806472e-01  8.98892422e-04 -4.71971198e-05
  -4.81865947e-02]] [[ 0.92535171 -0.21787918 -0.31024648]
 [ 0.21359787  0.97573425 -0.04815212]
 [ 0.31320946 -0.02171034  0.94943588]] [[ 9.48010302]
 [ 2.31152448]
 [14.71937031]] [[ -2.42003485 -14.4123777    2.90341315]
 [ 10.65133648  -3.0012281  -13.56738282]
 [ -0.11404328   9.75369425   0.26065529]] [[ 4.70486074e-07  2.79176041e-06 -2.54913364e-03]
 [-2.07119606e-06  5.81478240e-07  3.34956960e-03]
 [ 8.10274577e-04 -4.00502728e-03  1.00000000e+00]]
'''

print("-"*10)
print(cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, R, T, E, F)

with open("stereo_calib.json", 'w+', encoding='utf-8') as file:
    json.dump({
        "cameraMatrix1"         :   cameraMatrix1.tolist()   , 
        "distCoeffs1"           :   distCoeffs1.tolist()     , 
        "cameraMatrix2"         :   cameraMatrix2.tolist()   , 
        "distCoeffs2"           :   distCoeffs2.tolist() , 
        "R"                     :   R.tolist(), 
        "T"                     :   T.tolist(), 
        "E"                     :   E.tolist(), 
        "F"                     :   F.tolist()
    },
    file, 
    separators=(',', ':'), 
           sort_keys=True, 
           indent=4)

# with open("calibrate_2.json", "w", encoding='utf-8') as js:
    
#     for device in [cam_0, cam_1]:
#         matrixes = calibrate(device[1])
#         print(matrixes)
#         json.dump({
#             "device" : device[0],
#             "calibrate" : matrixes
#         }, js, separators=(',', ':'), 
#           sort_keys=True, 
#           indent=4)
