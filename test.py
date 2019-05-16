import os

import scipy as sp
import scipy.misc

import imreg_dft as ird


basedir = "figure" #os.path.join('..', 'examples')
# the TEMPLATE
im0 = sp.misc.imread(os.path.join(basedir, "3.jpg"), True)
# the image to be transformed
im1 = sp.misc.imread(os.path.join(basedir, "4.jpg"), True)
result = ird.similarity(im0, im1, numiter=3)
print(result)

assert "timg" in result
# Maybe we don't want to show plots all the time
if os.environ.get("IMSHOW", "yes") == "yes":
    import matplotlib.pyplot as plt
    ird.imshow(im0, im1, result['timg'])
    plt.show()
