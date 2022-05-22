import React, { Component, StrictMode } from "react";
import { TerminalContextProvider } from "react-terminal";
import "./style.css";
import Terminal from "terminal-in-react";

class TerminalUI extends Component {
  showMsg = () => "Welcome to the DIY Greenhouse Terminal";
  render() {
    return (
      <div
        style={{
          display: "flex",
          justifyContent: "center",
          alignItems: "center",
          height: "100vh",
        }}
      >
        <Terminal
          color="green"
          backgroundColor="black"
          barColor="purple"
          style={{
            fontWeight: "bold",
            fontSize: "1em",
            fontFamily: "monospace",
            color: "white",
          }}
          commands={{
            "open-google": () =>
              window.open("https://www.google.com/", "_blank"),
            showmsg: this.showMsg,
            popup: () => alert("Terminal in React"),
            color: {
              method: (args, print, runCommand) => {
                print(`The color is ${args._[0] || args.color}`);
              },
              options: [
                {
                  name: "color",
                  description: "The color the output should be",
                  defaultValue: "white",
                },
              ],
            },
            "type-text": (args, print, runCommand) => {
              const text = args.slice(1).join(" ");
              print("");
              for (let i = 0; i < text.length; i += 1) {
                setTimeout(() => {
                  runCommand(`edit-line ${text.slice(0, i + 1)}`);
                }, 100 * i);
              }
            },
          }}
          commandPassThrough={(cmd) =>
            `-PassedThrough:${cmd}: command not found`
          }
          descriptions={{
            "open-google": "opens google.com",
            showmsg: "shows a message",
            alert: "alert",
            popup: "alert",
          }}
          msg="Welcome to the terminal"
          hideTopBar={false}
          watchConsoleLogging={true}
        />
      </div>
    );
  }
}

export default TerminalUI;
