const path = require("path");
module.exports = {
  entry: "./webpack/entry.js",
  output: {
    path: path.resolve("assets/javascripts"),
    filename: "bundle.js",
  },
  module: {
    rules: [
      {
        test: /\.jsx?$/,
        exclude: /(node_modules)/,
        loader: "babel-loader", // "babel-loader" is also a legal name to reference
      },
    ],
  },
};