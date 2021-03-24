import xml.etree.ElementTree as ET
import os
import math
import numpy as np
import matplotlib.pyplot as plt

def write_file(lanelet_id_array,bound_array,x_array,y_array,yaw_array,file_name):
	# Folder "results" if not already there
	output_folder = "xml2txt"
	if not os.path.exists(output_folder):
		os.makedirs(output_folder)

	file_path = os.path.join(output_folder, file_name + "_lanelets_bounds" + ".txt")
	with open(file_path, 'w') as log_file: 
		log_file.write('lanelet_id, bound, x, y, yaw\n')
		for i in range(len(lanelet_id_array)):
			# log_file.write('%s, %s, %3.3f, %3.3f\n' %\
			# 	(lanelet_id_array[i],bound_array[i], x_array[i], y_array[i]))
			log_file.write('%s, %s, %s, %s, %f \n' %\
				(lanelet_id_array[i],bound_array[i], x_array[i], y_array[i], yaw_array[i]))

	file_path = os.path.join(output_folder, file_name + "_lanelets_description" + ".txt")
	with open(file_path, 'w') as log_file: 
		# log_file.write('lanelet_id, adjacentLeft_id, predecessor_id, successor_id\n')
		log_file.write('lanelet_id, adjacentLeft_id, predecessor_id, successor_id\n')
		for i in range(len(lanelet_id_array2)):
			# log_file.write('%s, %s, %3.3f, %3.3f\n' %\
			# 	(lanelet_id_array[i],bound_array[i], x_array[i], y_array[i]))
			log_file.write('%s, %s, %s ,%s\n' %\
				(lanelet_id_array2[i],adjacentLeft_array[i],predecessor_array[i],successor_array[i]))
	print('xml to txt SUCCESSFULLY converted!')




data_file = 'T_Junction.xml'
tree = ET.parse(data_file)
root = tree.getroot()

lanelet_id_array = []
bound_array = []
x_array = []
y_array = []


lanelet_id_array2    = []
predecessor_array  = []
successor_array    = []
adjacentLeft_array  = []
# counter = 0
for lanelet in root.iter('lanelet'):

	lanelet_id = lanelet.attrib['id']
	# print(lanelet.tag,' id = ',lanelet.attrib['id'])
	adjacentLeft_exist = False
	for child in lanelet.iter():
		if child.tag == "leftBound" or child.tag == "rightBound":
			for point in child.iter('point'):
				# print(point.tag)
				for x in point.iter('x'):
					# print(x.text)
					pass

				for y in point.iter('y'):
					# print(y.text)
					pass

				lanelet_id_array.append(lanelet_id)
				bound_array.append(child.tag)
				x_array.append(x.text)
				y_array.append(y.text)

				# counter+=1
				# print(counter)
				# if not (counter % 150):
				# 	np_x_array = np.asarray(x_array)
				# 	x_array_float = np_x_array.astype(np.float)

				# 	np_y_array = np.asarray(y_array)
				# 	y_array_float = np_y_array.astype(np.float)

				# 	plt.plot(x_array_float, y_array_float)
				# 	plt.show()

		if child.tag == "predecessor":
			lanelet_id_array2.append(lanelet_id)
			# print("lane",lanelet_id)
			predecessor_id = child.attrib['ref']
			predecessor_array.append(predecessor_id)

		if child.tag == "successor":
			successor_id = child.attrib['ref']
			successor_array.append(successor_id)

		if child.tag == "adjacentLeft":
			adjacentLeft_exist = True
			adjacentLeft_id = child.attrib['ref']

	if adjacentLeft_exist:
		adjacentLeft_array.append(adjacentLeft_id)
		adjacentLeft_array.append(adjacentLeft_id)
	elif not adjacentLeft_exist:
		adjacentLeft_array.append(-1)
		adjacentLeft_array.append(-1)



yaw_array = np.zeros(len(x_array))
print(yaw_array)

# for i in range(lanelet_id):

np_x_array = np.asarray(x_array)
x_array_float = np_x_array.astype(np.float)

np_y_array = np.asarray(y_array)
y_array_float = np_y_array.astype(np.float)

np_lanelet_id_array = np.asarray(lanelet_id_array)
np_lanelet_id_array_float = np_lanelet_id_array.astype(np.float)

unique_lanelet_id_array = np.unique(np_lanelet_id_array_float)
# print("unique_lanelet_id_array = ",unique_lanelet_id_array)
for unique_lanelet_id in unique_lanelet_id_array:
	lanelet_indexes = [index for index, value in enumerate(np_lanelet_id_array_float) if value == unique_lanelet_id]
	# print("================================")
	# print("unique_lanelet_id = ",unique_lanelet_id)
	# print("lanelet_indexes = ",lanelet_indexes)

	# # lanelet_indexes_left_bound = bound_array[lanelet_indexes]
	# lanelet_bounds = [bound_array[index] for index in lanelet_indexes]
	# # lanelet_indexes_left_bound = [index for index, value in enumerate(np_lanelet_id_array_float) if value == unique_lanelet_id[0]]
	# print("lanelet_bounds = ",lanelet_bounds)

	# indexes_for_lanelet_left_bound_indexes = [index for index, value in enumerate(lanelet_bounds) if value == 'leftBound']
	# # print("indexes_for_lanelet_left_bound_indexes = ",indexes_for_lanelet_left_bound_indexes)
	# lanelet_indexes_left_bound = [lanelet_indexes[index] for index in indexes_for_lanelet_left_bound_indexes]
	# print("lanelet_indexes_left_bound = ",lanelet_indexes_left_bound)

	# indexes_for_lanelet_right_bound_indexes = [index for index, value in enumerate(lanelet_bounds) if value == 'rightBound']
	# # print("indexes_for_lanelet_right_bound_indexes = ",indexes_for_lanelet_right_bound_indexes)
	# lanelet_indexes_right_bound = [lanelet_indexes[index] for index in indexes_for_lanelet_right_bound_indexes]
	# print("lanelet_indexes_right_bound = ",lanelet_indexes_right_bound)


	# left_bound_x  =  [x_array_float[index] for index in lanelet_indexes_left_bound]
	# left_bound_y  =  [y_array_float[index] for index in lanelet_indexes_left_bound]
	# right_bound_x =  [x_array_float[index] for index in lanelet_indexes_right_bound]
	# right_bound_y =  [y_array_float[index] for index in lanelet_indexes_right_bound]

	# print(left_bound_x)
	# print(lanelet_indexes)
	for j in lanelet_indexes:#range(len(left_bound_x)):
		print("in")
		if j >= lanelet_indexes[-1]:
			print("in1")
			yaw_array[j] = np.pi/2 - np.arctan2((y_array_float[j]-y_array_float[j-1]) , (x_array_float[j]-x_array_float[j-1]))
		else:
			print("in2")
			yaw_array[j] = np.pi/2 - np.arctan2((y_array_float[j+1]-y_array_float[j]) , (x_array_float[j+1]-x_array_float[j]))
		

		# print(len(y_array_float))
		# print(j-1)
		


print(yaw_array)
	

# print(x_array)
# print(y_array)
# plt.plot(x_array, y_array)
# plt.show()	
file_name = "T_Junction"		
write_file(lanelet_id_array,bound_array,x_array,y_array,yaw_array,file_name)

# np_x_array = np.asarray(x_array)
# x_array_float = np_x_array.astype(np.float)

# np_y_array = np.asarray(y_array)
# y_array_float = np_y_array.astype(np.float)

# print(x_array_float)
# plt.plot(x_array_float, y_array_float)
# plt.show()


def draw_line(x, y, angle):
    r = 1  # or whatever fits you
    plt.arrow(x, y, r*np.cos(angle), r*np.sin(angle), length_includes_head = True, head_width = 0.2)


for i in range(len(x_array_float)):
  draw_line(x_array_float[i],y_array_float[i],-np.pi/2 - yaw_array[i])

plt.show()