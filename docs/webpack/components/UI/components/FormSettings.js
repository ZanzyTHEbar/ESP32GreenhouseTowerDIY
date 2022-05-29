import React from "react";
import {
  Grid,
  List,
  ListItem,
  ListItemIcon,
  ListItemText,
  ListSubheader,
  Icon,
  FormControlLabel,
  Switch,
  Typography,
} from "@mui/material";
import HassIO from "../../icons/homeassistant.svg";
import BluetoothIcon from "@mui/icons-material/Bluetooth";
import WifiIcon from "@mui/icons-material/Wifi";
import SystemUpdateAltIcon from "@mui/icons-material/SystemUpdateAlt";
import SmartButtonTwoToneIcon from "@mui/icons-material/SmartButtonTwoTone";

const FormSettings = () => {
  const [state, setState] = React.useState({
    mqtt: false,
    wifi: true,
    bluetooth: true,
    ota: false,
    mdns: false,
    hassio: false,
    light_sensor: false,
    ds18b20: false,
    dht: false,
  });

  const handleButtonChange = (event) => {
    setState({
      ...state,
      [event.target.name]: event.target.checked,
    });
  };

  const HASSIO = () => {
    return (
      <div>
        <Icon sx={{ fill: "#757575" }}>
          <HassIO />
        </Icon>
      </div>
    );
  };

  return (
    <div>
      <Grid container spacing={3}>
        <Grid item sm={12}>
          <List
            sx={{
              width: "100%",
              maxWidth: 800,
              bgcolor: "background.paper",
            }}
            subheader={<ListSubheader>Settings</ListSubheader>}
          >
            <ListItem>
              <ListItemIcon>
                <WifiIcon />
              </ListItemIcon>
              <ListItemText id="switch-list-label-wifi" primary="WiFi" />
              <FormControlLabel
                control={
                  <Switch
                    checked={state.wifi ? true : false && !state.bluetooth}
                    onChange={handleButtonChange}
                    name="wifi"
                    inputProps={{
                      "aria-label": "secondary checkbox",
                    }}
                  />
                }
              />
            </ListItem>
            <ListItem>
              <ListItemIcon>
                <SmartButtonTwoToneIcon />
              </ListItemIcon>
              <ListItemText id="switch-list-label-mqtt" primary="MQTT" />
              <FormControlLabel
                control={
                  <Switch
                    checked={state.wifi ? state.mqtt : false}
                    onChange={handleButtonChange}
                    name="mqtt"
                    inputProps={{
                      "aria-label": "secondary checkbox",
                    }}
                  />
                }
              />
            </ListItem>
            <ListItem>
              <ListItemIcon>
                <SystemUpdateAltIcon />
              </ListItemIcon>
              <ListItemText id="switch-list-label-ota" primary="WiFi-OTA" />
              <FormControlLabel
                control={
                  <Switch
                    checked={state.wifi ? state.ota : false}
                    onChange={handleButtonChange}
                    name="ota"
                    inputProps={{
                      "aria-label": "secondary checkbox",
                    }}
                  />
                }
              />
            </ListItem>
            <ListItem>
              <ListItemIcon>
                <HASSIO />
              </ListItemIcon>
              <ListItemText
                id="switch-list-label-hassio"
                primary="Home Assistant"
              />
              <FormControlLabel
                control={
                  <Switch
                    checked={state.wifi ? state.hassio : false}
                    onChange={handleButtonChange}
                    name="hassio"
                    inputProps={{
                      "aria-label": "secondary checkbox",
                    }}
                  />
                }
              />
            </ListItem>
            <ListItem>
              <ListItemIcon>
                <BluetoothIcon />
              </ListItemIcon>
              <ListItemText
                id="switch-list-label-bluetooth"
                primary={
                  <React.Fragment>
                    <Typography color="inherit">Bluetooth</Typography>
                    <p>
                      <u>
                        {"("}
                        {"OTA"}
                        <b>{" only"}</b>{" "}
                      </u>
                      {")"}
                    </p>
                  </React.Fragment>
                }
              />
              <FormControlLabel
                control={
                  <Switch
                    checked={!state.wifi && state.bluetooth}
                    onChange={handleButtonChange}
                    name="bluetooth"
                    inputProps={{
                      "aria-label": "secondary checkbox",
                    }}
                  />
                }
              />
            </ListItem>
          </List>
        </Grid>
      </Grid>
    </div>
  );
};

export default FormSettings;
