# @author: Mario Sandoval
# CSCE 465
import operator

# open the file and read the lines from the file
text = open('ciphertext.txt','r');
textLines = text.readlines();

# start a dictionary and get the frequencies
freq = {}

for line in textLines: # for every line 
	for char in line: # for every character
		freq[char] = freq.get(char, 0) + 1 # add 1 to the count of the charcter

#sort the dictionary
freq = sorted(freq.items(), key=operator.itemgetter(1))

#print the result
print("The frenquency of letters in the text are:\n" + str(freq))

