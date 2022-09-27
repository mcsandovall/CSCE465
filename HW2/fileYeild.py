# @author: Mario Sandoval

from fileinput import filename
from tokenize import Double


file_name = raw_input("Enter filename: ")
file = open(file_name, 'r');

file_length = 0
chars_mod = 0

for lines in file.readlines():
    file_length += len(lines)
    # I am assuming all letter in the passage are capitalized
    mod_chars =  [char for char in lines if ord(char) != ord(' ') and char != '\n' and ord(char) < ord("A") or ord(char) > ord("Z")]
    chars_mod += len(mod_chars)

yeild = (1 - (float(float(chars_mod)/float(file_length)))) * 100
print("Total Size of the file is: " +  str(file_length))
print("The total ammount of corrupted chars are: " + str(chars_mod))
print("The yeild of recoverable content is: " + str(yeild) + "%")
    

