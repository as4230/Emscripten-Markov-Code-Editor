# I used this to number the data so I can pass it into the parser easier. 
import os

# Set the directory where your C files are located
directory = "/home/adams/windows-home/projects/editor/public/data"

# Change to the directory
os.chdir(directory)

# Loop through each file in the directory
for i, filename in enumerate(os.listdir(directory)):
    # Check if the file is a C file
    if filename.endswith(".c"):
        # Create the new filename with the sequential numbering system
        new_filename = str(i+1) + ".c"
        # Rename the file
        os.rename(filename, new_filename)