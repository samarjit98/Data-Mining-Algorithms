import cv2
import random

def min_dist(img, clus_cent, i, j):
	pos = 0
	dist = 10000000.00
	for k in range(len(clus_cent)):
		if abs(img[clus_cent[k][0]][clus_cent[k][1]] - img[i][j]) < dist :
			dist = abs(img[clus_cent[k][0]][clus_cent[k][1]] - img[i][j])
			pos = k
	return pos

def mean(lst):
	sum = 0
	for i in range(len(lst)):
		sum = sum + lst[i]
	return int(sum / len(lst))

def kmeans(img, k):
	clus_cent = []
	for i in range(k):
		point = []
		point.append(random.randint(0, img.shape[0]))
		point.append(random.randint(0, img.shape[1]))
		clus_cent.append(point)

	final_cluster = []
	for d in range(20):
		cluster = [[] for i in range(k)]

		for i in range(img.shape[0]):
			for j in range(img.shape[1]):
				pos = min_dist(img, clus_cent, i, j)
				point = []
				point.append(i)
				point.append(j)
				cluster[pos].append(point)

		for i in range(k):
			clus = cluster[i]
			if len(clus)==0:
				continue
			lst1 = []
			lst2 = []
			for j in range(len(clus)):
				lst1.append(clus[j][0])
				lst2.append(clus[j][1])

			mean1 = mean(lst1)
			mean2 = mean(lst2)

			clus_cent[i][0] = mean1
			clus_cent[i][1] = mean2

		
		final_cluster = cluster

	for z in range(len(final_cluster)):
		clus = final_cluster[z]
		color = random.randint(0, 255)

		for i in range(len(clus)):
			img[clus[i][0]][clus[i][1]]=color

	return img

if __name__ == "__main__":
	img = cv2.imread('img2.jpeg')
	img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	img = kmeans(img, 5)
	cv2.imwrite('img_out.jpeg', img)





