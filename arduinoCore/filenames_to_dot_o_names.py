#!/usr/bin/python3

with open("correct_ar_file_order") as input_file:
    names = input_file.readlines()

o_filenames = [i.replace("\n", "").split("/")[-1].split(".")[0] + ".o" for i in names if ".c" in i or ".S" in i]

with open("dot_o_filenames.txt", "w") as output_file:
    output_file.write(" ".join(o_filenames) + "\n")
