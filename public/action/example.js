/**
 * This is an example of skulpt interpreting python code. The idea is user code would end up here 
 * as a string and the JS would use skulpt to run it.
 */
const Sk = require('skulpt');
const output = [];

// Set up Skulpt
Sk.configure({
  output: (text) => output.push(text),
});

// Define the Python code as a string
const pythonCode = 'print("Hello, World!!!!")';

// Run the Python code
Sk.misceval.asyncToPromise(() => Sk.importMainWithBody('<stdin>', false, pythonCode, true))
  .then(() => {
    // Log the output
    console.log(output.join(''));
  })
  .catch((err) => {
    // Handle any errors
    console.error(err.toString());
  });