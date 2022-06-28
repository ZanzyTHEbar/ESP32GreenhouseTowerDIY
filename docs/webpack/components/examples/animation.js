import * as React from "react";
import "./style.css";
import Styled, { keyframes } from "styled-components";
import { AppBar } from "@mui/material/";

class Animations extends React.Component {
  constructor(props) {
    super(props);
  }

  Header = (props) => {
    /* Animations */
    const gradientbackground = keyframes`
    0% {
        background-position: 0% 50%;
    }
    50% {
        background-position: 100% 50%;
    }
    100% {
        background-position: 0% 50%;
    }`;

    const GradientBackgroundAnimated = Styled.h3`
    animation-name: ${gradientbackground};
    animation-duration: 5s;
    animation-iteration-count: infinite;
    `;

    return (
      <GradientBackgroundAnimated>{props.label}</GradientBackgroundAnimated>
    );
  };

  render() {
    return (
      <div>
        <AppBar position="relative" className="gradientBackground">
          <this.Header label="Hello World" />
        </AppBar>
      </div>
    );
  }
}

export default Animations;
