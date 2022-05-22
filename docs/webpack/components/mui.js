import React, { Component, StrictMode } from "react";
import { Button, ButtonGroup, Checkbox, Typography, AppBar, Card, CardActionArea, CardActions, CardContent, CardMedia, CssBaseline, Grid, Toolbar, Container } from "@mui/material/";
import SaveIcon from "@mui/icons-material/SaveAltTwoTone";
import  DeleteIcon from "@mui/icons-material/Delete";
import Checkout from "./CheckOut";
import "./style.css";

class MUI extends Component {
  showMsg = () => "Setting up Firmware Image";
  CheckBoxState = () => {
    const [checked, setChecked] = React.useState(true);
     return (
       <div>
         <Checkbox
           checked={checked}
           color="primary"
           onChange={(e) => setChecked(e.target.checked)}
           value="checkedA"
           inputProps={{ "aria-label": "primary checkbox" }}
         />
       </div>
     );
  };

  MUIButtonGroup = () => {
    return (
      <div>
        <ButtonGroup
          variant="contained"
          color="primary"
          aria-label="contained primary button group"
        >
          <Button>One</Button>
          <Button>Two</Button>
          <Button>Three</Button>
        </ButtonGroup>
      </div>
    );
  };
  render() {
    return (
      <div
        style={{
          display: "flex",
        }}
        className="react-mui-ui"
      >
        <CssBaseline />
        <AppBar position="relative">
          <Toolbar>
            <SaveIcon />
            <Typography variant="h6" color="inherit" noWrap>
            Save Settings
            </Typography>
          </Toolbar>
        </AppBar>
        {/* <div>
          <this.CheckBoxState />
          <this.MUIButtonGroup />
        </div> */}
        {/* <div>
          <Checkout />
        </div> */}
      </div>
    );
  }
}

export default MUI;

// // Telnet.run();
// const commands = Com;
// export default function App() {
//   return <div className="App"></div>;
// }
