const path = require("path");
module.exports = {
  mode: "production",
  entry: "./webpack/entry.js",
  output: {
    path: path.resolve("assets/javascripts"),
    filename: "bundle.js",
  },
  module: {
    rules: [
      {
        test: /\.js|jsx?$/,
        exclude: /(node_modules)/,
        loader: "babel-loader", // "babel-loader" is also a legal name to reference
      },
      {
        test: /\.css$/,
        use: ["style-loader", "css-loader"],
      },
      {
        test: /\.svg$/,
        use: ["@svgr/webpack"],
      },
    ],
  },
};
