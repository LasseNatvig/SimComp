#!/usr/bin/env python
import os
import time
import numpy
import matplotlib.pyplot as plt

#define a function that will just open your file containing numbers and get 
#all data into a list of floats
def getNumbers(file):
  numbers = []
  with open(file, 'r') as numbers_file:
    for line in numbers_file.readlines():
      numbers.append(float(line.strip('\n').strip()))
  return numbers
os.system('TYPE SimComp.inp | SimComp_Console.exe C:\\code\\simcomp\\_Simulator\\_sasm\\ 1> results.txt' )
results = open("results.txt", "r")
MIPSrate = open("results_measured.txt", "w")
sample = 0 
xValues = []
yValues = []
lineNo = 1
for line in results: 
	if line != '\n':
		Words = line.split()
		if Words[0] == "@":
			# MIPS rate is only on every second line
			if (lineNo % 2) == 0 :
				sample = sample + 1
				MIPSrate.write(str(Words[1]) + '\n')
				xValues.append(sample)
				yValues.append(Words[1])
			# else do nothing
			lineNo = lineNo + 1	
MIPSrate.close()

plt.title('Simulator MIPS-rate for repeated runs') 
plt.plot(xValues, yValues, color = "green")
plt.ylim(ymax=100, ymin = 20)
plt.show()

num_list = getNumbers('results_measured.txt')
#num_list = getNumbers(MIPSrate)
print 'Maximum is %g' %max(num_list)
print 'Average is %g' %numpy.mean(num_list)
print 'Standard deviation is %g' %numpy.std(num_list)