#!/usr/bin/python3
from os import system

choice = 0
security = {1: "Making Secure", 2: "Making insecure"}
while choice not in [1, 2]:
	print("Do you want to make the arduino secure or insecure?")
	print("Enter 1 for Secure.")
	print("Enter 2 for Insecure.")
	choice = int(input("> "))
print(security[choice])

if choice == 1: # use the core.a file with the stack erase attribute
	system("cd ./arduinoCore/ && make clean && make && cd .. && cp ./lockboxes/secureLockbox.cpp lockbox.cpp && make clean && make upload")
else: # use the core.a file without the stack erase attribute 
	system("make clean && cp ./insecureCore/core.a ./core.a && cp ./lockboxes/insecureLockbox.cpp lockbox.cpp && make upload")

