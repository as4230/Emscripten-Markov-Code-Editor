import os
import tokenize

# Loop through all input files
with open("public/backend/data.txt", "w") as fp_out:
    for i in range(1, 1263):
        filename = f"./data_python/{i}.py"
        if not os.path.isfile(filename):
            continue

        #print("FILE: " + filename + '\n')
        with open(filename, "rb") as fp_in:
            # Parse the Python file
            try:
                tokens = tokenize.tokenize(fp_in.readline)
            except tokenize.TokenError as e:
                print(f"Error in file {filename}: {e}")
                continue
            except IndentationError as e:
                print(f"IndentationError in file {filename} on line {e.args[1][1]}: {e}")
                continue

            # Print the tokens
            for token in tokens:
                if token.type == tokenize.NAME:
                    fp_out.write(token.string + "/CODE_DELIMITER/")
                elif token.type == tokenize.OP and token.string in ('(', ')', ':', ';'):
                    fp_out.write(token.string + "/CODE_DELIMITER/")
            fp_out.write("\n")