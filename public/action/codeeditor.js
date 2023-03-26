function linenumbers() {
  let lineNumber = 1;
  // loop through each codearea element
  document.querySelectorAll(".codearea").forEach((codearea) => {
    // get the ID of the corresponding linenumbers element
    const id = `line-${lineNumber}`;
    // try to get the existing linenumbers element with the ID
    let linenumbers = document.getElementById(id);
    if (linenumbers) {
      // a linenumbers element already exists, so just update its text content
      linenumbers.textContent = lineNumber;
    } else {
      // create a new linenumbers element with the ID and the line number as its text content
      linenumbers = document.createElement("div");
      linenumbers.classList.add("linenumbers");
      linenumbers.id = id;
      linenumbers.textContent = lineNumber;

      // insert the linenumbers element before the codearea element
      codearea.parentNode.insertBefore(linenumbers, codearea);
    }
    linenumbers.style.position = "absolute";
    linenumbers.style.left = "0";
    linenumbers.style.top = `${codearea.offsetTop}px`;
    linenumbers.style.height = `${codearea.offsetHeight}px`;

    // increment the line number
    lineNumber++;
  });

  // set the display property of the linenumbers elements
  document.querySelectorAll(".linenumbers").forEach((linenumbers) => {
    linenumbers.style.display = "inline-block";
  });

  // set the text-align property of the parent container
  const parentContainer = document.querySelector(".editor");
  parentContainer.style.textAlign = "left";
}

linenumbers();

// Get all the code area elements
const codeAreas = document.querySelectorAll(".codearea");
var codeArea;

// Attach an event listener to handle keypresses for each code area, including new ones
document.addEventListener("keydown", (event) => {
  // Get all the code area elements
  const codeAreas = document.querySelectorAll(".codearea");

  // Check if the target element is a code area
  if (event.target.classList.contains("codearea")) {
    codeArea = event.target;

    // Get the code in the text area
    const code = codeArea.textContent;

    // Get the current cursor position
    const cursorPosition = getCaretPosition(codeArea);

    // Handle different keypresses
    switch (event.key) {
      case "Tab":
        // Insert a tab character at the cursor position
        event.preventDefault();
        codeArea.textContent =
          code.slice(0, cursorPosition) + "\t" + code.slice(cursorPosition);
        setCaretPosition(codeArea, cursorPosition + 1);
        break;
      case "Enter":
        event.preventDefault();
        const area = document.createElement("div");
        area.classList.add("codearea");
        area.contentEditable = true;
        codeArea.parentNode.insertBefore(area, codeArea.nextSibling);
        // update line numbers for subsequent lines
        let currentLineNumber = parseInt(
          codeArea.nextSibling.previousSibling.textContent
        );
        document.querySelectorAll(".linenumbers").forEach((linenumbers) => {
          if (parseInt(linenumbers.textContent) >= currentLineNumber) {
            linenumbers.textContent = parseInt(linenumbers.textContent) + 1;
            linenumbers.id = `line-${parseInt(linenumbers.textContent)}`;
          }
        });

        linenumbers();
        setCaretPosition(area, -1);
        break;
      case "Backspace":
        // If the code area is empty, remove it and update line numbers
        if (codeArea.textContent === "") {
          event.preventDefault();

          const previousCodeArea =
            codeArea.previousElementSibling.previousSibling;
          if (
            (previousCodeArea &&
              previousCodeArea.classList &&
              previousCodeArea.classList.contains("codearea")) ||
            (previousCodeArea.classList &&
              previousCodeArea.classList.contains("linenumbers"))
          ) {
            let currentLineNumber = parseInt(
              codeArea.previousSibling.textContent
            );

            codeArea.parentNode.removeChild(codeArea);

            // Remove excess line numbers
            const lineNumbers = document.querySelectorAll(".linenumbers");
            if (lineNumbers.length >= codeAreas.length) {
              const excessLines = lineNumbers.length - codeAreas.length + 1;
              for (let i = 1; i <= excessLines; i++) {
                lineNumbers[lineNumbers.length - i].remove();
              }
            }

            // Update line numbers for subsequent lines
            document.querySelectorAll(".linenumbers").forEach((linenumbers) => {
              if (parseInt(linenumbers.textContent) >= currentLineNumber) {
                linenumbers.textContent = parseInt(linenumbers.textContent) - 1;
                linenumbers.id = `line-${parseInt(linenumbers.textContent)}`;
              }
            });

            linenumbers();

            setCaretPosition(
              previousCodeArea,
              previousCodeArea.textContent.length
            );
          }
        }
        break;
      case "ArrowUp":
        // Move the cursor to the same character index on the previous line
        event.preventDefault();
        const previousLine =
          codeArea.previousElementSibling.previousElementSibling;
        if (previousLine && previousLine.classList.contains("codearea")) {
          const previousCode = previousLine.textContent;
          const cursorIndex = getCaretPosition(codeArea);
          const lastCharIndex = previousCode.length;
          const sameCharIndex = Math.min(cursorIndex, lastCharIndex);
          setCaretPosition(previousLine, sameCharIndex);
        }
        break;
      case "ArrowDown":
        event.preventDefault();
        const nextLineNum = codeArea.nextElementSibling;
        if (nextLineNum && nextLineNum.classList.contains("linenumbers")) {
          const nextLineCodeArea = nextLineNum.nextElementSibling;
          if (
            nextLineCodeArea &&
            nextLineCodeArea.classList.contains("codearea")
          ) {
            const codeLength = nextLineCodeArea.textContent.length;
            setCaretPosition(nextLineCodeArea, 0);
            setCaretPosition(
              nextLineCodeArea,
              Math.min(cursorPosition, codeLength)
            );
          }
        }
        break;
      default:
        // Do nothing
        break;
    }
  }
});

document.querySelector(".editor").addEventListener("paste", (event) => {
  event.preventDefault();
  const text = (event.clipboardData || window.clipboardData).getData("text");
  // Split the pasted text by newlines and create a new code area for each line
  const lines = text.split(/\r?\n/);
  for (let i = lines.length - 1; i >= 0; i--) {
    const area = document.createElement("div");
    area.classList.add("codearea");
    area.contentEditable = true;
    area.textContent = lines[i];
    codeArea.parentNode.insertBefore(area, codeArea.nextSibling);
  }
  // Remove excess line numbers
  const lineNumbers = document.querySelectorAll(".linenumbers");
  if (lineNumbers.length >= codeAreas.length) {
    const excessLines = lineNumbers.length - codeAreas.length + 1;
    for (let i = 1; i <= excessLines; i++) {
      lineNumbers[lineNumbers.length - i].remove();
    }
  }
  // Update line numbers for subsequent lines
  let currentLineNumber = parseInt(
    codeArea.nextSibling.previousSibling.textContent
  );
  document.querySelectorAll(".linenumbers").forEach((linenumbers) => {
    if (parseInt(linenumbers.textContent) >= currentLineNumber) {
      linenumbers.textContent = parseInt(linenumbers.textContent) + 1;
      linenumbers.id = `line-${parseInt(linenumbers.textContent)}`;
    }
  });
  linenumbers();
});

// Helper function to get the caret position in a contenteditable element
function getCaretPosition(element) {
  const range = window.getSelection().getRangeAt(0);
  const preCaretRange = range.cloneRange();
  preCaretRange.selectNodeContents(element);
  preCaretRange.setEnd(range.endContainer, range.endOffset);
  return preCaretRange.toString().length;
}

// Helper function to set the caret position in a contenteditable element
function setCaretPosition(element, position) {
  const range = document.createRange();
  const sel = window.getSelection();

  // Handle case for Enter key
  if (position === -1) {
    range.setStart(element, 0);
    range.collapse(true);
  } else if (element.childNodes.length > 0) {
    range.setStart(element.childNodes[0], position);
    range.collapse(true);
  } else {
    range.setStart(element, 0);
    range.collapse(true);
  }
  sel.removeAllRanges();
  sel.addRange(range);
}

let worker = new Worker("/action/models-worker.js");

// send a message to the worker to start loading the models
worker.postMessage({ type: "loadModels" });

//example on how generating tokens will work
// worker.postMessage({
//   type: 'generateToken',
//   previous: 'for',
//   source: 'i'
// });

var orderOneLoaded = false;
var orderTwoLoaded = false;
var orderThreeLoaded = false;
var orderFourLoaded = false;
var orderFiveLoaded = false;

// handle messages from the worker
worker.onmessage = async function (event) {
  //console.log('Received message from worker:', event.data.type);
  switch (event.data.type) {
    case "modelsLoaded":
      worker.postMessage({ type: "loadOrderOne" });
      worker.postMessage({ type: "loadOrderTwo" });
      worker.postMessage({ type: "loadOrderThree" });
      worker.postMessage({ type: "loadOrderFour" });
      worker.postMessage({ type: "loadOrderFive" });
      // the models have been loaded, now we can something with it
      break;
    case "tokensOneGenerated":
      //tokens have been generated, do something with them
      //console.log('Generated tokens:', event.data.tokens);
      if (orderOneLoaded) {
        let suggestionButton = document.querySelector("#order-one button");
        if (suggestionButton) {
          suggestionButton.textContent = event.data.tokens
            .filter((token) => token !== "")
            .join(" ");
        }
      }
      break;
    case "tokensTwoGenerated":
      //tokens have been generated, do something with them
      //console.log('Generated tokens:', event.data.tokens);
      if (orderTwoLoaded) {
        let suggestionButton = document.querySelector("#order-two button");
        if (suggestionButton) {
          suggestionButton.textContent = event.data.tokens
            .filter((token) => token !== "")
            .join(" ");
        }
      }
      break;
    case "tokensThreeGenerated":
      //tokens have been generated, do something with them
      //console.log('Generated tokens:', event.data.tokens);
      if (orderThreeLoaded) {
        let suggestionButton = document.querySelector("#order-three button");
        if (suggestionButton) {
          suggestionButton.textContent = event.data.tokens
            .filter((token) => token !== "")
            .join(" ");
        }
      }
      break;
    case "tokensFourGenerated":
      //tokens have been generated, do something with them
      //console.log('Generated tokens:', event.data.tokens);
      if (orderFourLoaded) {
        let suggestionButton = document.querySelector("#order-four button");
        if (suggestionButton) {
          suggestionButton.textContent = event.data.tokens
            .filter((token) => token !== "")
            .join(" ");
        }
      }
      break;
    case "tokensFiveGenerated":
      //tokens have been generated, do something with them
      //console.log('Generated tokens:', event.data.tokens);
      if (orderFiveLoaded) {
        let suggestionButton = document.querySelector("#order-five button");
        if (suggestionButton) {
          suggestionButton.textContent = event.data.tokens
            .filter((token) => token !== "")
            .join(" ");
        }
      }
      break;
    case "orderOneLoaded":
      orderOneLoaded = true;
      // Get the <li> element with an id of "order-one"
      const orderOne = document.querySelector("#order-one");

      // Create a new <button> element
      const buttonOne = document.createElement("button");
      buttonOne.classList.add("suggestion"); // Add the "suggestion" class to the button
      buttonOne.textContent = ""; // Set the button text to the placeholder text

      // Set the onclick function to call addSuggestion() with the button's text content
      buttonOne.onclick = function () {
        addSuggestion(buttonOne.textContent);
      };

      // Create a new <li> element to wrap the button
      const buttonOneLi = document.createElement("li");
      buttonOneLi.appendChild(buttonOne); // Add the button to the new <li> element

      // Replace the original <li> element with the new one that contains the button
      orderOne.replaceWith(buttonOneLi);
      buttonOneLi.setAttribute("id", "order-one");
      let orderOneCircle = document.querySelector("#order-one-heading .circle");
      orderOneCircle.classList.add("loaded");
      orderOneCircle.classList.remove("not-loaded");
      break;
    case "orderTwoLoaded":
      orderTwoLoaded = true;
      // Get the <li> element with an id of "order-two"
      const orderTwo = document.querySelector("#order-two");

      // Create a new <button> element
      const buttonTwo = document.createElement("button");
      buttonTwo.classList.add("suggestion"); // Add the "suggestion" class to the button
      buttonTwo.textContent = ""; // Set the button text to the placeholder text

      // Set the onclick function to call addSuggestion() with the button's text content
      buttonTwo.onclick = function () {
        addSuggestion(buttonTwo.textContent);
      };

      // Create a new <li> element to wrap the button
      const buttonTwoLi = document.createElement("li");
      buttonTwoLi.appendChild(buttonTwo); // Add the button to the new <li> element

      // Replace the original <li> element with the new one that contains the button
      orderTwo.replaceWith(buttonTwoLi);
      buttonTwoLi.setAttribute("id", "order-two");
      let orderTwoCircle = document.querySelector("#order-two-heading .circle");
      orderTwoCircle.classList.add("loaded");
      orderTwoCircle.classList.remove("not-loaded");
      break;
    case "orderThreeLoaded":
      orderThreeLoaded = true;
      // Get the <li> element with an id of "order-Three"
      const orderThree = document.querySelector("#order-three");

      // Create a new <button> element
      const buttonThree = document.createElement("button");
      buttonThree.classList.add("suggestion"); // Add the "suggestion" class to the button
      buttonThree.textContent = ""; // Set the button text to the placeholder text

      // Set the onclick function to call addSuggestion() with the button's text content
      buttonThree.onclick = function () {
        addSuggestion(buttonThree.textContent);
      };

      // Create a new <li> element to wrap the button
      const buttonThreeLi = document.createElement("li");
      buttonThreeLi.appendChild(buttonThree); // Add the button to the new <li> element

      // Replace the original <li> element with the new one that contains the button
      orderThree.replaceWith(buttonThreeLi);
      buttonThreeLi.setAttribute("id", "order-three");
      let orderThreeCircle = document.querySelector(
        "#order-three-heading .circle"
      );
      orderThreeCircle.classList.add("loaded");
      orderThreeCircle.classList.remove("not-loaded");
      break;
    case "orderFourLoaded":
      orderFourLoaded = true;
      // Get the <li> element with an id of "order-Four"
      const orderFour = document.querySelector("#order-four");

      // Create a new <button> element
      const buttonFour = document.createElement("button");
      buttonFour.classList.add("suggestion"); // Add the "suggestion" class to the button
      buttonFour.textContent = ""; // Set the button text to the placeholder text

      // Set the onclick function to call addSuggestion() with the button's text content
      buttonFour.onclick = function () {
        addSuggestion(buttonFour.textContent);
      };

      // Create a new <li> element to wrap the button
      const buttonFourLi = document.createElement("li");
      buttonFourLi.appendChild(buttonFour); // Add the button to the new <li> element

      // Replace the original <li> element with the new one that contains the button
      orderFour.replaceWith(buttonFourLi);
      buttonFourLi.setAttribute("id", "order-four");
      let orderFourCircle = document.querySelector(
        "#order-four-heading .circle"
      );
      orderFourCircle.classList.add("loaded");
      orderFourCircle.classList.remove("not-loaded");
      break;
    case "orderFiveLoaded":
      orderFiveLoaded = true;
      // Get the <li> element with an id of "order-Five"
      const orderFive = document.querySelector("#order-five");

      // Create a new <button> element
      const buttonFive = document.createElement("button");
      buttonFive.classList.add("suggestion"); // Add the "suggestion" class to the button
      buttonFive.textContent = ""; // Set the button text to the placeholder text

      // Set the onclick function to call addSuggestion() with the button's text content
      buttonFive.onclick = function () {
        addSuggestion(buttonFive.textContent);
      };

      // Create a new <li> element to wrap the button
      const buttonFiveLi = document.createElement("li");
      buttonFiveLi.appendChild(buttonFive); // Add the button to the new <li> element

      // Replace the original <li> element with the new one that contains the button
      orderFive.replaceWith(buttonFiveLi);
      buttonFiveLi.setAttribute("id", "order-five");
      let orderFiveCircle = document.querySelector(
        "#order-five-heading .circle"
      );
      orderFiveCircle.classList.add("loaded");
      orderFiveCircle.classList.remove("not-loaded");
      break;
    default:
      // unknown message type
      break;
  }
};

/**
 * I am using skulpt to interpret python code.
 * This is mostly a toy project to learn about emscripten but I still want it to work!
 */

async function runCode() {
  const output = [];
  const outputArea = document.getElementById("output-area");
  outputArea.textContent = "";

  // Set up Skulpt
  Sk.configure({
    output: (text) => {
      output.push(text); // push new text to the output array
    },
  });

  const codeareas = document.querySelectorAll(".codearea");
  const pythonCode = Array.from(codeareas)
    .map((area) => area.textContent)
    .filter((code) => code !== "") // exclude empty strings
    .join("\n");
  Sk.misceval
    .asyncToPromise(() =>
      Sk.importMainWithBody("<stdin>", false, pythonCode, true)
    )
    .then(() => {
      // Log the output
      outputArea.textContent = output.join("");
    })
    .catch((err) => {
      // Handle any errors
      outputArea.textContent = err.toString();
      console.error(err.toString());
    });
}

let codeAreasContainer = document.getElementsByClassName("editor")[0];

let lastActiveCodeArea = null;

codeAreasContainer.addEventListener("focusin", function (event) {
  const codeArea = event.target.closest(".codearea");
  if (codeArea) {
    codeArea.classList.add("active-codearea");
    lastActiveCodeArea = codeArea;
  }
});

codeAreasContainer.addEventListener("focusout", function (event) {
  const codeArea = event.target.closest(".codearea");
  if (codeArea) {
    codeArea.classList.remove("active-codearea");
  }
});

//My way of providing suggestions is to get the line the user is currently on,
//use a lexer to parse through the line and tokenize it, and
//pass the last two tokens in to the markov chain.
//A more advanced algorithm might be able to do it whereever the user is typing and be more useful but
//a markov chain gets no context besides the tokens that are passed in

//lexer

function monitorCodeAreas() {
  let activeCodeArea = document.querySelector(".active-codearea");

  if (activeCodeArea) {
    let code = activeCodeArea.textContent;
    let lexpy_lang = lexpy.gen(lex);
    const lexer = new lex.Lexer(lexpy_lang, code);
    let token = lexer.get_token();
    let tokens = [];
    while (token !== null) {
      if (token.text !== " ") {
        tokens.push(token.text);
      }
      token = lexer.get_token();
    }

    let lastFiveTokens;
    if (tokens.length === 0) {
      lastFiveTokens = ["", "", "", "", ""];
    } else if (tokens.length === 1) {
      lastFiveTokens = ["", "", "", "", tokens[0]];
    } else if (tokens.length === 2) {
      lastFiveTokens = ["", "", "", tokens[0], tokens[1]];
    } else if (tokens.length === 3) {
      lastFiveTokens = ["", "", tokens[0], tokens[1], tokens[2]];
    } else if (tokens.length === 4) {
      lastFiveTokens = ["", tokens[0], tokens[1], tokens[2], tokens[3]];
    } else {
      lastFiveTokens = tokens.slice(-5); // Get the last 5 tokens
    }

    if (lastFiveTokens.length < 5) {
      const missingTokens = 5 - lastFiveTokens.length;
      for (let i = 0; i < missingTokens; i++) {
        lastFiveTokens.unshift(""); // Add empty strings at the beginning
      }
    }
    
    console.log(lastFiveTokens);

    if (orderOneLoaded) {
      worker.postMessage({
        type: "generateTokensOne",
        source: lastFiveTokens[lastFiveTokens.length - 1],
      });
    }
    if (orderTwoLoaded) {
      worker.postMessage({
        type: "generateTokensTwo",
        previous: lastFiveTokens[lastFiveTokens.length - 2],
        source: lastFiveTokens[lastFiveTokens.length - 1],
      });
    }
    if (orderThreeLoaded) {
      worker.postMessage({
        type: "generateTokensThree",
        previous1: lastFiveTokens[lastFiveTokens.length - 3],
        previous2: lastFiveTokens[lastFiveTokens.length - 2],
        source: lastFiveTokens[lastFiveTokens.length - 1],
      });
    }
    if (orderFourLoaded) {
      worker.postMessage({
        type: "generateTokensFour",
        previous1: lastFiveTokens[lastFiveTokens.length - 4],
        previous2: lastFiveTokens[lastFiveTokens.length - 3],
        previous3: lastFiveTokens[lastFiveTokens.length - 2],
        source: lastFiveTokens[lastFiveTokens.length - 1],
      });
    }
    if (orderFiveLoaded) {
      worker.postMessage({
        type: "generateTokensFive",
        previous1: lastFiveTokens[lastFiveTokens.length - 5],
        previous2: lastFiveTokens[lastFiveTokens.length - 4],
        previous3: lastFiveTokens[lastFiveTokens.length - 3],
        previous4: lastFiveTokens[lastFiveTokens.length - 2],
        source: lastFiveTokens[lastFiveTokens.length - 1],
      });
    }
  }

  //get the last two tokens that the user is on every 4 seconds
  setTimeout(monitorCodeAreas, 4000);
}

// call the function to start monitoring code areas
monitorCodeAreas();

function addSuggestion(suggestion) {
  // Get the last active codearea element or the first .codearea element if lastActiveCodeArea is null
  const codeArea = lastActiveCodeArea || document.querySelector(".codearea");
  // Append the suggestion to the content of the codearea element
  codeArea.textContent += " " + suggestion;
}

let orderOneCircle = document.querySelector("#order-one-heading .circle");
orderOneCircle.classList.add("not-loaded");
let orderTwoCircle = document.querySelector("#order-two-heading .circle");
orderTwoCircle.classList.add("not-loaded");
let orderThreeCircle = document.querySelector("#order-three-heading .circle");
orderThreeCircle.classList.add("not-loaded");
let orderFourCircle = document.querySelector("#order-four-heading .circle");
orderFourCircle.classList.add("not-loaded");
let orderFiveCircle = document.querySelector("#order-five-heading .circle");
orderFiveCircle.classList.add("not-loaded");
