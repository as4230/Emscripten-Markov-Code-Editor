const express = require("express");
const path = require("path");

const app = express();

const staticOptions = {
  dotfiles: "ignore",
  etag: false,
  extensions: ["htm", "html"],
  index: false,
  redirect: false,
};

app.use(
  express.static("public", { ...staticOptions, type: "application/javascript" })
);
app.use(
  express.static("backend", { ...staticOptions, type: "application/wasm" })
);
app.use(
  express.static("backend", {
    ...staticOptions,
    type: "application/wasm",
    extensions: ["data"],
  })
);

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/public/templates/code.html");
});

// const mime = require('mime');

app.get("/backend/mastermind.data", (req, res) => {
  const filePath = path.join(__dirname, "backend", "mastermind.data");
  res.setHeader("Content-Type", "application/wasm");
  res.sendFile(filePath);
});

app.listen(8000, () => {
  console.log("Server started on port 8000");
});
