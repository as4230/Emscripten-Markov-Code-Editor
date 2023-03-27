# Emscripten-Markov-Code-Editor
---------------------------------------------
## Description
This toy project is a python code editor with simple code suggestions. The main purpose of this was for me to explore how to implement a simple algorithm (markov chain) and learn how to use emscripten. The code suggestion algorithm is just a markov chain that I implemented myself in C and then compiled using emscripten. I actually created 5 orders of markov chains but the code itself is very similar just with more tokens. I exported the methods needed then had it work with my main frontend JS to generate tokens. The code editor itself is extremely simple because I created it myself and I am not the best with design. Again, this isn't meant to be a practical code editor but just a learning experience for me.

## Technologies used 
--------------------
- Skulpt: Used for executing Python code in the browser
- Emscripten: Used for compiling the markov chains implemented in C to JavaScript
- Express: Used for serving my HTML/CSS/JS to my browser

## Instructions
--------------------
You need to have Node.js installed in order to start the express server to use this project. I use the express server to serve some files like the emscripten created .data file so it won't work if you just open the HTML. I personally have Node.js v18.8.0 installed. You can find more info here: https://nodejs.org/en/download 
- Clone the project into a local repository or download the zip for this project (under the Code tab)
- Go to the project in a terminal/command line where you have node.js installed and go to the top level of the project.
- Enter "node server.js"
- Go to localhost:8000 and view!

## Markov Chain
--------------------
A markov chain is used to show a sequence of events where the next depends on the current and previous events. There are orders that determine how many previous events are used to make the next event. For example, a order one markov chain would only take one input, let's say "let" (python statement to assign variables) is passed in and the markov chain would generate the next token off of that like "x" or some other variable name that was passed in for training.

A markov chain is really simple and intuitive so here is a simplified example in the context of python code with a order 2 chain (2 tokens for the next transition).
Training data: "for i in range(10): print(i)"
Tokens: for, i, in, range, (10):, print(i)
Probability Table: 
| Input Tokens | Next Token | Probability |
|--------------|------------|-------------|
| (for, i)     | (in)       | 1.0         |
| (i, in)      | (range)    | 1.0         |
| (in, range)  | ((10:))    | 1.0         |
| (range, (10:))| (print(i)) | 1.0        |
| ((10:), print(i)) | (None) | 1.0      |
| (print(i), '')  | (None)   | 1.0       |

So in this case there is only 1 instance of all of these tokens. The probability comes in when There are more versions like if there was a another version of a for loop where this transition (i, in) -> (range) was instead (i, in) -> (enumerate) then when you enter the tokens "i in" then there is a 50-50 chance of either getting range or enumerate. 

## Shortcomings/Improvments
--------------------
I am pretty much done with this project but there are some things that I could have done better that I will keep in mind for future projects.
- More Organized JS: I should have split apart my main JS and organized code better. The main JS does a lot of functionality that I feel I could have split off.
- Encoded Data Files: I wrote my trained markov chain data directly to .bin files with no encoding so the time it takes to load the files back into memory takes longer than it should. 
- Parallelism: I put the workload of loading models/generating tokens in a web worker but it still requires users to wait for every model to load before generating tokens. Some models (like an order one markov chain) loads very quickly before the higher order models and I could have created a way for the web worker to load models in the background while handling token generation requests. Emscripten optimization helps a lot though and the all models only take about 2 minutes to load on my machine.

## Data
--------------------
I got my data from leetcode answers. I did this hoping the code suggestions would be more helpful and not too specific. You can find that data here: https://github.com/cnkyrpsgl/leetcode
