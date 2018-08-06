#!/usr/bin/python3
from os import getcwd, system
file_paths = []
with open("coreStrings.txt") as file:
    for line in file.readlines()[:-1]:
        tokens = line.split()
        for token in tokens:
            for file_type in [".c", ".s", ".S"]:
                if file_type in token and ".o" not in token:
                    file_paths.append(token[1:len(token)-1])
for file_path in file_paths:
    file_name = file_path.split("/")[-1::][0]
    current_dir = getcwd()
    command = "cp {} {}/arduinoCore/{}".format(file_path, current_dir, file_name)
    system(command)
