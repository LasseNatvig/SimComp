#!/usr/bin/env python
import os
os.system('TYPE SimComp_C_asm0.inp | SimComp_Console.exe C:\\code\\simcomp\\_Simulator\\_sasm\\ 1> results.txt' )
os.system('myDiff.exe results.txt correct_asm0.out 1> diff.txt')
os.system('copy results.txt results_0.txt')
file = open("diff.txt", "r")
for line in file: 
	print line,

os.system('TYPE SimComp_C_asm1.inp | SimComp_Console.exe C:\\code\\simcomp\\_Simulator\\_sasm\\ 1> results.txt' )
os.system('myDiff.exe results.txt correct_asm1.out 1> diff.txt')
os.system('copy results.txt results_1.txt')
file = open("diff.txt", "r")
for line in file: 
	print line, 

# ... asm3b is based on asm3 but with loop values 1000 and 10000 to make faster execution
os.system('TYPE SimComp_C_asm3b.inp | SimComp_Console.exe C:\\code\\simcomp\\_Simulator\\_sasm\\ 1> results.txt' )
os.system('myDiff.exe results.txt correct_asm3b.out 1> diff.txt')
os.system('copy results.txt results_3.txt')
file = open("diff.txt", "r")
for line in file: 
 	print line,

