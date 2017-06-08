import numpy as np
import cv2
import matplotlib.pyplot as plt
import argparse
import os
import math

s = '''
16 8 7 6 6 1 1 1 1 1 1 1 1 1 1 1
7 7 6 6 1 1 1 1 1 1 1 1 1 1 1 30
7 6 6 1 1 1 1 1 1 1 1 1 1 1 30 28
6 8 1 1 1 1 1 1 1 1 1 1 1 32 35 29
8 1 1 1 1 1 1 1 1 1 1 1 32 35 32 28
1 1 1 1 1 1 1 1 1 1 1 35 40 42 40 35
1 1 1 1 1 1 1 1 1 1 35 44 42 40 35 31
1 1 1 1 1 1 1 1 1 35 44 44 50 53 52 45
1 1 1 1 1 1 1 1 31 34 44 55 53 52 45 39
1 1 1 1 1 1 1 31 34 40 41 47 52 45 52 50
1 1 1 1 1 1 30 32 36 41 47 52 54 57 50 46
1 1 1 1 1 36 32 36 44 47 52 57 60 60 55 50
1 1 1 1 36 39 42 44 48 52 57 61 60 60 55 51
1 1 1 39 42 47 48 46 59 57 56 55 52 51 54 51
1 1 41 46 47 48 48 49 53 56 53 50 51 52 51 50
1 43 47 47 48 48 49 57 57 56 50 52 52 51 50 50
'''
l = s.split()
l = [int(i) for i in l]
l = np.array(l)
table = np.resize(l, (16, 16))

def compress(file, factor):
    #Check the file existence
    if os.path.isfile(file):
        image = cv2.imread(file, 0)
        #plt.imshow(image)
        #plt.show()
        n = 16
        indexorder = sorted(((x,y) for x in range(n) for y in range(n)), key = lambda p: (p[0]+p[1], -p[1] if (p[0]+p[1]) % 2 else p[1]) )
        #print(table)
        h, w = image.shape[:2]
        newH = h
        newW = w
        compress_list = []
        count_list = []
        #Extend image's size to multiple of 16
        while newH % 16 != 0:
            newH += 1
        while newW % 16 != 0:
            newW += 1

        whole = np.zeros((newH, newW), np.uint8)
        newImage = np.zeros((newH, newW), np.uint8)
        #Assign each value to new image
        for i in range(h):
            for j in range(w):
                newImage[i, j] = image[i, j]
                
        for i in range(0, newH, 16):
            for j in range(0, newW, 16):
                #Take 16*16 block
                imsub = np.zeros((16, 16))
                imsub[:16, :16] = newImage[i: i + 16, j: j + 16]
                vis = np.zeros((16,16), np.float32)
                vis[:16, :16] = imsub
                #print(imsub)
                #DCT process
                dct = cv2.dct(vis)
                #Divide process
                result = np.divide(dct, table * factor)
                result = np.round(result)
                result = result.astype(int)
                #print(result)
                zigzag = [result[m, n] for m, n in indexorder]
                #Count the last consecutive zeros
                c = countZero(zigzag)
                c = [str(int(s)) for s in c]
                cnt = c.count("0")
                count_list.append(cnt)
                c += ["$", str(cnt)]
                compress_list.append(c)
                whole[i: i + 16, j: j + 16] = result

        plt.imshow(whole)
        plt.show()
        cv2.imwrite('result{}.bmp'.format(factor), whole)
        with open('result{}.txt'.format(factor), 'w') as f:
            for l in compress_list:
                for c in l:
                    f.write(c + " ")
                f.write("\n")
        f.close()
        avg = sum(count_list) / len(count_list)
        PSNR = calculate(image, whole)
        with open('static.txt', 'a') as f:
            f.write("{}:\n".format(file))
            f.write("Factor: {}, Average zero: {}, File size: {} bytes, PSNR: {}\n".format(factor, avg, os.stat('result{}.txt'.format(factor)).st_size, PSNR))


def countZero(l):
    find = None
    for i, bit in enumerate(reversed(l)):
        if int(bit) != 0:
            find = i
            break
    return l[: len(l) - find]

def calculate(frame1, frame2):
    h, w = frame1.shape[:2]
    size = h * w
    l = []
    for hi in range(h):
        for wi in range(w):
            value = int(frame1[hi, wi] - frame2[hi, wi])
            l.append(value ** 2)
    MSE = sum(l) / size
    PSNR = 10 * math.log10(255 * 255 / MSE)
    return PSNR


#compress('./test1.bmp', 1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process an image.')
    parser.add_argument('file', metavar='N', type=str,
                    help='an file for compress')
    args = parser.parse_args()
    for i in [1, 2, 4]:
        compress(args.file, i)




