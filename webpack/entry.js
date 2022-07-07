import React, { Component, StrictMode } from "react";
import { createRoot } from "react-dom/client";
import Mui from "./components/mui";

const rootElement = document.getElementById("react-ui-build");
const root = createRoot(rootElement);

root.render(
  <StrictMode>
    <Mui />
  </StrictMode>
);
