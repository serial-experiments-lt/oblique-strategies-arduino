# core
import os
import random

if __name__ == '__main__':
	# load text file
	strategies = open('OBLIQUE.txt', 'r').readlines()
	for i in range(len(strategies)):
		strategies[i] = strategies[i].split('\n')[0]
	# shuffle
	selection = random.choices(strategies, k=20)
	# make new file
	filepath = os.path.normpath(f'{__file__}/../strategies.hpp')
	if os.path.exists(filepath):
		os.remove(filepath)
	with open(filepath, 'at') as new_file:
		# add imports
		new_file.write(r'#pragma once' + '\n')
		new_file.write('\n')
		# start array
		new_file.write(r'String strategies[] = {' + '\n')
		# loop over subset	
		for strategy in selection:
			new_file.write(f'	"{strategy}",' + '\n')
		# end array
		new_file.write(r'};' + '\n')
		new_file.close()
	
	exit()

