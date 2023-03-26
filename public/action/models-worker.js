let wasmModule;
var startTime;
var endTime;
self.addEventListener("message", async function (event) {
  startTime = performance.now();
  switch (event.data.type) {
    case "loadModels":
      loadModels().then(async function () {
        endTime = performance.now();
        console.log(`WASM loaded in ${(endTime - startTime) / 1000} seconds`);
        //console.log(wasmModule);
        self.postMessage({ type: "modelsLoaded" });
      });
      break;
    case "generateTokensOne":
      //endTime = performance.now();
      let token_array_one = generateTokensOne(event.data.source);
      self.postMessage({ type: "tokensOneGenerated", tokens: token_array_one });
      //console.log(`Order 1 Tokens Generated in ${(endTime - startTime) / 1000} seconds`);
      break;
    case "generateTokensTwo":
      //endTime = performance.now();
      let token_array_two = generateTokensTwo(
        event.data.previous,
        event.data.source
      );
      self.postMessage({ type: "tokensTwoGenerated", tokens: token_array_two });
      //console.log(`Order 2 Tokens Generated in ${(endTime - startTime) / 1000} seconds`);
      break;
    case "generateTokensThree":
      //endTime = performance.now();
      let token_array_three = generateTokensThree(
        event.data.previous1,
        event.data.previous2,
        event.data.source
      );
      self.postMessage({
        type: "tokensThreeGenerated",
        tokens: token_array_three,
      });
      //console.log(`Order 3 Tokens Generated in ${(endTime - startTime) / 1000} seconds`);
      break;
    case "generateTokensFour":
      //endTime = performance.now();
      let token_array_four = generateTokensFour(
        event.data.previous1,
        event.data.previous2,
        event.data.previous3,
        event.data.source
      );
      self.postMessage({
        type: "tokensFourGenerated",
        tokens: token_array_four,
      });
      //console.log(`Order 4 Tokens Generated in ${(endTime - startTime) / 1000} seconds`);
      break;
    case "generateTokensFive":
      //endTime = performance.now();
      let token_array_five = generateTokensFive(
        event.data.previous1,
        event.data.previous2,
        event.data.previous3,
        event.data.previous4,
        event.data.source
      );
      self.postMessage({
        type: "tokensFiveGenerated",
        tokens: token_array_five,
      });
      //console.log(`Order 5 Tokens Generated in ${(endTime - startTime) / 1000} seconds`);
      break;
    case "loadOrderOne":
      loadOrderOne("model_1.bin")
        .then(async function () {
          endTime = performance.now();
          console.log(
            `Order 1 Markov Chain Loaded in ${
              (endTime - startTime) / 1000
            } seconds`
          );
          self.postMessage({ type: "orderOneLoaded" });
        })
        .catch(function (err) {
          console.error("Error loading Order 1 Markov Chain:", err);
        });
      break;
    case "loadOrderTwo":
      loadOrderTwo("model_2.bin")
        .then(async function () {
          endTime = performance.now();
          console.log(
            `Order 2 Markov Chain Loaded in ${
              (endTime - startTime) / 1000
            } seconds`
          );
          self.postMessage({ type: "orderTwoLoaded" });
        })
        .catch(function (err) {
          console.error("Error loading Order 2 Markov Chain:", err);
        });
      break;
    case "loadOrderThree":
      loadOrderThree("model_3.bin")
        .then(async function () {
          endTime = performance.now();
          console.log(
            `Order 3 Markov Chain Loaded in ${
              (endTime - startTime) / 1000
            } seconds`
          );
          self.postMessage({ type: "orderThreeLoaded" });
        })
        .catch(function (err) {
          console.error("Error loading Order 3 Markov Chain:", err);
        });
      break;
    case "loadOrderFour":
      loadOrderFour("model_4.bin")
        .then(async function () {
          endTime = performance.now();
          console.log(
            `Order 4 Markov Chain Loaded in ${
              (endTime - startTime) / 1000
            } seconds`
          );
          self.postMessage({ type: "orderFourLoaded" });
        })
        .catch(function (err) {
          console.error("Error loading Order 4 Markov Chain:", err);
        });
      break;
    case "loadOrderFive":
      loadOrderFive("model_5.bin")
        .then(async function () {
          endTime = performance.now();
          console.log(
            `Order 5 Markov Chain Loaded in ${
              (endTime - startTime) / 1000
            } seconds`
          );
          self.postMessage({ type: "orderFiveLoaded" });
        })
        .catch(function (err) {
          console.error("Error loading Order 5 Markov Chain:", err);
        });
      break;
    default:
      // unknown message type
      break;
  }
});

//focus here....
function loadModels() {
  importScripts("../backend/mastermind.js");
  console.log("Loading WASM Now....");
  return myModule(/* optional default settings */)
    .then(function (Module) {
      //console.log("WASM loaded successfully");
      wasmModule = Module;
      return Module;
    })
    .catch(function (err) {
      console.log("WASM failed to load:", err);
      throw err;
    });
}

async function loadOrderOne(file) {
  console.log("Loading Order 1 Markov Chain");
  return new Promise(function (resolve, reject) {
    let loadOne = wasmModule.cwrap("load_transition_list_one", null, [
      "string",
    ]);
    loadOne(file);
    resolve();
  });
}

async function loadOrderTwo(file) {
  console.log("Loading Order 2 Markov Chain");
  return new Promise(function (resolve, reject) {
    let loadTwo = wasmModule.cwrap("load_transition_list_two", null, [
      "string",
    ]);
    loadTwo(file);
    resolve();
  });
}

async function loadOrderThree(file) {
  console.log("Loading Order 3 Markov Chain");
  return new Promise(function (resolve, reject) {
    let loadThree = wasmModule.cwrap("load_transition_list_three", null, [
      "string",
    ]);
    loadThree(file);
    resolve();
  });
}

async function loadOrderFour(file) {
  console.log("Loading Order 4 Markov Chain");
  return new Promise(function (resolve, reject) {
    let loadFour = wasmModule.cwrap("load_transition_list_four", null, [
      "string",
    ]);
    loadFour(file);
    resolve();
  });
}

async function loadOrderFive(file) {
  console.log("Loading Order 5 Markov Chain");
  return new Promise(function (resolve, reject) {
    let loadFive = wasmModule.cwrap("load_transition_list_five", null, [
      "string",
    ]);
    loadFive(file);
    resolve();
  });
}

function generateTokensOne(source) {
  let generate = wasmModule.cwrap("generate_tokens_one", "number", ["string"]);
  // Call the exported C function and get the pointer to the C array
  let tokensPointer = generate(source);

  // Create a JavaScript array to hold the tokens
  let tokens = [];

  // Loop through the C array and add the tokens to the JavaScript array
  for (let i = 0; i < 2; i++) {
    tokens.push(
      wasmModule.UTF8ToString(wasmModule.HEAP32[(tokensPointer >> 2) + i])
    );
  }
  wasmModule._free(tokensPointer);
  return tokens;
}

function generateTokensTwo(previous, source) {
  let generate = wasmModule.cwrap("generate_tokens_two", "number", [
    "string",
    "string",
  ]);
  // Call the exported C function and get the pointer to the C array
  let tokensPointer = generate(previous, source);

  // Get the length of the C array. The array is always 4 though for my order 2 chain so doing this shouldn't be necessary
  //let tokensLength = wasmModule.HEAPU32[tokensPointer / 4];

  // Create a JavaScript array to hold the tokens
  let tokens = [];

  // Loop through the C array and add the tokens to the JavaScript array
  for (let i = 0; i < 4; i++) {
    //It internally returns an array of numbers so we need to shift it.
    //getValue allows us to access it without worrying about bit shifts but both of these will get you the same output.
    //console.log(wasmModule.UTF8ToString(wasmModule.getValue(tokensPointer + i * 4, 'i32')));
    tokens.push(
      wasmModule.UTF8ToString(wasmModule.HEAP32[(tokensPointer >> 2) + i])
    );
  }
  //The strings that are returned is trained data in the model so I don't need to free it.
  //I do need to free the array returned however
  wasmModule._free(tokensPointer);
  return tokens;
}

function generateTokensThree(previous1, previous2, source) {
  let generate = wasmModule.cwrap("generate_tokens_three", "number", [
    "string",
    "string",
    "string",
  ]);
  // Call the exported C function and get the pointer to the C array
  let tokensPointer = generate(previous1, previous2, source);

  // Create a JavaScript array to hold the tokens
  let tokens = [];

  // Loop through the C array and add the tokens to the JavaScript array
  for (let i = 0; i < 6; i++) {
    tokens.push(
      wasmModule.UTF8ToString(wasmModule.HEAP32[(tokensPointer >> 2) + i])
    );
  }
  wasmModule._free(tokensPointer);
  return tokens;
}

function generateTokensFour(previous1, previous2, previous3, source) {
  let generate = wasmModule.cwrap("generate_tokens_four", "number", [
    "string",
    "string",
    "string",
    "string",
  ]);
  // Call the exported C function and get the pointer to the C array
  let tokensPointer = generate(previous1, previous2, previous3, source);

  // Create a JavaScript array to hold the tokens
  let tokens = [];

  // Loop through the C array and add the tokens to the JavaScript array
  for (let i = 0; i < 8; i++) {
    tokens.push(
      wasmModule.UTF8ToString(wasmModule.HEAP32[(tokensPointer >> 2) + i])
    );
  }
  wasmModule._free(tokensPointer);
  return tokens;
}

function generateTokensFive(
  previous1,
  previous2,
  previous3,
  previous4,
  source
) {
  let generate = wasmModule.cwrap("generate_tokens_five", "number", [
    "string",
    "string",
    "string",
    "string",
    "string",
  ]);
  // Call the exported C function and get the pointer to the C array
  let tokensPointer = generate(
    previous1,
    previous2,
    previous3,
    previous4,
    source
  );

  // Create a JavaScript array to hold the tokens
  let tokens = [];

  // Loop through the C array and add the tokens to the JavaScript array
  for (let i = 0; i < 10; i++) {
    tokens.push(
      wasmModule.UTF8ToString(wasmModule.HEAP32[(tokensPointer >> 2) + i])
    );
  }
  wasmModule._free(tokensPointer);
  return tokens;
}
