import cv2, script
import numpy as np

img1 = cv2.imread("figure/1.jpg")
img_gray = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
img2 = cv2.imread("figure/2.jpg", 0)
w, h = img2.shape[::-1]
print("width: %d, height: %d" % (w, h))
croplen = min(w, h) // 2
cropped = img2[h//2-croplen//2:h//2+croplen//2, w//2-croplen//2:w//2+croplen//2]
print("cropped.shape:", cropped.shape)

cv2.namedWindow("show",0)
cv2.resizeWindow("show", 960, 540)
# cv2.imshow('show', cropped)
# cv2.waitKey(0)

res = cv2.matchTemplate(img_gray, cropped, cv2.TM_CCOEFF_NORMED)
threshold = 0.8
loc = np.where( res >= threshold)

for pt in zip(*loc[::-1]):
	cv2.rectangle(img1, pt, (pt[0] + croplen, pt[1] + croplen), (7,249,151), 2)

cv2.imshow('show', img1)
cv2.waitKey(0)
