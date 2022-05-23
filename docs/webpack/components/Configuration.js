import * as React from "react";
import {
  Accordion,
  AccordionSummary,
  AccordionDetails,
  Button,
  CircularProgress,
  Divider,
  Backdrop,
  FormControlLabel,
  FormGroup,
  FormLabel,
  FormControl,
  FormHelperText,
  TextField,
  Switch,
  InputLabel,
  Autocomplete,
  Input,
  OutlinedInput,
  ButtonGroup,
  Box,
  Icon,
  List,
  ListSubheader,
  ListItem,
  ListItemIcon,
  ListItemText,
  Checkbox,
  Typography,
  AppBar,
  Card,
  CardActionArea,
  CardActions,
  CardContent,
  CardMedia,
  CssBaseline,
  Grid,
  Toolbar,
  Container,
  Paper,
  Alert,
  AlertColor,
  AlertTitle,
  AlertDescription,
  AlertIcon,
  AlertAction,
  AlertActionGroup,
  Stepper,
  Step,
  StepLabel,
  StepContent,
  Link,
  StepConnector,
  StepConnectorClasses,
  LinearProgress,
} from "@mui/material/";
import ExpandMoreIcon from "@mui/icons-material/ExpandMoreRounded";
import WifiIcon from "@mui/icons-material/Wifi";
import BluetoothIcon from "@mui/icons-material/Bluetooth";
import SystemUpdateAltIcon from "@mui/icons-material/SystemUpdateAlt";
import SmartButtonTwoToneIcon from "@mui/icons-material/SmartButtonTwoTone";
import HassIO from "./icons/homeassistant.svg";

const boardNames = [
  "Custom PCB",
  "ESP32_Devkit_C",
  "Adafruit_Feather_Huzzah",
  "ESP32_DevKit_32s",
  "ESP32_Sketch_Board",
  "DOIT_ESP32",
  "ESP32_CAM",
];
const firmwareVersions = ["main", "nightly", "beta"];

function HASSIO() {
  return (
    <div>
      <Icon sx={{ fill: "#757575" }}>
        <HassIO />
      </Icon>
    </div>
  );
}

function BackDrop() {
  const [open, setOpen] = React.useState(false);
  const handleClose = () => {
    setOpen(false);
  };
  const handleToggle = () => {
    setOpen(!open);
  };
  return (
    <div>
      <Button onClick={handleToggle}>Show backdrop</Button>
      <Backdrop
        sx={{ color: "#fff", zIndex: (theme) => theme.zIndex.drawer + 1 }}
        open={open}
        onClick={handleClose}
      >
        <CircularProgress color="inherit" />
      </Backdrop>
    </div>
  );
}

export default function Configuration() {
  const [value_boardNames, setValue_boardNames] = React.useState(boardNames[0]);
  const [value_firmwareVersion, setValue_firmwareVersion] = React.useState(
    firmwareVersions[0]
  );
  const [inputValue_boardNames, setInputValue_boardNames] = React.useState("");
  const [inputValue_firmwareVersion, setInputValue_firmwareVersion] =
    React.useState("");

  const [state, setState] = React.useState({
    mqtt: false,
    wifi: false,
    bluetooth: false,
    ota: false,
    mdns: false,
    hassio: false,
    light_sensor: false,
    sht31: false,
    ds18b20: false,
    dht11: false,
    dht22: false,
    dht21: false,
  });

  const handleChange = (event) => {
    setState({
      ...state,
      [event.target.name]: event.target.checked,
    });
  };

  return (
    <React.Fragment>
      <Typography variant="h6" gutterBottom>
        Versioning
      </Typography>
      <form>
        <Grid container spacing={3}>
          <Grid item sm={12}>
            {/* <TextField
              required
              id="firmwareVersion"
              label="Firmware Version"
              fullWidth
              autoComplete="firmwareVersion"
            /> */}
            <FormControl
              component="fieldset"
              variant="outlined"
              required
              fullWidth
            >
              <Alert
                severity="error"
                role="info"
                variant="filled"
                color="error"
              >
                <AlertTitle>Warning</AlertTitle>
                It is recommended to use the <strong>most recent</strong>{" "}
                version
              </Alert>
              <br></br>
              <FormGroup>
                <Autocomplete
                  value={value_firmwareVersion}
                  onChange={(event, newValue) => {
                    setValue_firmwareVersion(newValue);
                  }}
                  inputValue={inputValue_firmwareVersion}
                  onInputChange={(event, newInputValue) => {
                    setInputValue_firmwareVersion(newInputValue);
                  }}
                  id="controllable-states"
                  options={firmwareVersions}
                  fullWidth
                  renderInput={(params) => (
                    <TextField {...params} required label="Firmware version" />
                  )}
                />
                <br></br>
                <Autocomplete
                  value={value_boardNames}
                  onChange={(event, newValue) => {
                    setValue_boardNames(newValue);
                  }}
                  inputValue={inputValue_boardNames}
                  onInputChange={(event, newInputValue) => {
                    setInputValue_boardNames(newInputValue);
                  }}
                  id="controllable-states"
                  options={boardNames}
                  fullWidth
                  renderInput={(params) => (
                    <TextField {...params} required label="Board Name" />
                  )}
                />
                <FormHelperText sx={{ color: "#0971f1" }}>
                  If unsure - choose ESP32_Devkit_C <br></br>
                  If using the PCB design files from this repo, choose Custom
                  PCB
                </FormHelperText>
                <br></br>
                <Typography variant="h6" gutterBottom>
                  Features
                </Typography>
                <Accordion>
                  <AccordionSummary
                    expandIcon={<ExpandMoreIcon />}
                    aria-controls="panel1a-content"
                    id="panel1a-header"
                  >
                    <Typography>Configure Features</Typography>
                  </AccordionSummary>
                  <AccordionDetails>
                    <Typography>
                      Customize your Build Configuration with the below
                      settings. Certain Features require the wifi module to be
                      enabled. This project uses LAN for communication, so you
                      will need to enable the wifi module. If you really do not
                      want to enable the wifi module, you can disable it here,
                      and bluetooth with be enabled automatically.
                    </Typography>
                    <Alert
                      severity="info"
                      role="info"
                      variant="filled"
                      color="info"
                    >
                      <AlertTitle>Info</AlertTitle>
                      The Bluetooth software module{" "}
                      <strong>only functions</strong> with Bluetooth-based OTA,{" "}
                      <strong>not </strong>
                      sending or monitoring data.
                    </Alert>
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
                            <ListItemText
                              id="switch-list-label-wifi"
                              primary="WiFi"
                            />
                            <FormControlLabel
                              control={
                                <Switch
                                  checked={state.wifi ? true : false && !state.bluetooth}
                                  onChange={handleChange}
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
                            <ListItemText
                              id="switch-list-label-mqtt"
                              primary="MQTT"
                            />
                            <FormControlLabel
                              control={
                                <Switch
                                  checked={state.wifi ? state.mqtt : false}
                                  onChange={handleChange}
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
                            <ListItemText
                              id="switch-list-label-ota"
                              primary="WiFi-OTA"
                            />
                            <FormControlLabel
                              control={
                                <Switch
                                  checked={state.wifi ? state.ota : false}
                                  onChange={handleChange}
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
                                  onChange={handleChange}
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
                              primary="Bluetooth"
                            />
                            <FormControlLabel
                              control={
                                <Switch
                                  checked={!state.wifi && state.bluetooth}
                                  onChange={handleChange}
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
                  </AccordionDetails>
                </Accordion>
              </FormGroup>
            </FormControl>
          </Grid>
          <Grid item sm={12}>
            <Grid container spacing={2}>
              <Grid item xs={12} sm={6}>
                <Paper
                  sx={{
                    padding: "1rem",
                    margin: "1rem",
                    width: "100%",
                    maxWidth: 800,
                    bgcolor: "background.paper",
                  }}
                  variant="outlined"
                >
                  <Typography variant="h6" gutterBottom>
                    Humidity Sensors (Optional)
                  </Typography>
                  <br></br>
                  <Autocomplete
                    value={value_boardNames}
                    onChange={(event, newValue) => {
                      setValue_boardNames(newValue);
                    }}
                    inputValue={inputValue_boardNames}
                    onInputChange={(event, newInputValue) => {
                      setInputValue_boardNames(newInputValue);
                    }}
                    id="controllable-states"
                    options={boardNames}
                    fullWidth
                    renderInput={(params) => (
                      <TextField {...params} label="Board Name" />
                    )}
                  />
                  <FormHelperText sx={{ color: "#0971f1" }}>
                    If unsure - choose ESP32_Devkit_C <br></br>
                    If using the PCB design files from this repo, choose Custom
                    PCB
                  </FormHelperText>
                  <br></br>
                </Paper>
              </Grid>
              <Grid item xs={12} sm={6}>
                <Paper
                  variant="outlined"
                  sx={{
                    padding: "1rem",
                    margin: "1rem",
                    width: "100%",
                    maxWidth: 800,
                    bgcolor: "background.paper",
                  }}
                >
                  <Typography variant="h6" gutterBottom>
                    Temperature Sensors (Optional)
                  </Typography>
                  <br></br>
                  <Autocomplete
                    value={value_boardNames}
                    onChange={(event, newValue) => {
                      setValue_boardNames(newValue);
                    }}
                    inputValue={inputValue_boardNames}
                    onInputChange={(event, newInputValue) => {
                      setInputValue_boardNames(newInputValue);
                    }}
                    id="controllable-states"
                    options={boardNames}
                    fullWidth
                    renderInput={(params) => (
                      <TextField {...params} label="Board Name" />
                    )}
                  />
                  <FormHelperText sx={{ color: "#0971f1" }}>
                    If unsure - choose ESP32_Devkit_C <br></br>
                    If using the PCB design files from this repo, choose Custom
                    PCB
                  </FormHelperText>
                  <br></br>
                </Paper>
              </Grid>
            </Grid>
          </Grid>
        </Grid>
      </form>
    </React.Fragment>
  );
}
