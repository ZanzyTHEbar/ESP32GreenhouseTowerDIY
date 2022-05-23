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
  Radio,
  RadioGroup,
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
  Tooltip,
  Stack,
  IconButton,
  Link,
  StepConnector,
  StepConnectorClasses,
  LinearProgress,
} from "@mui/material/";
import { alpha, styled } from "@mui/material/styles";
import { pink } from "@mui/material/colors";
import ExpandMoreIcon from "@mui/icons-material/ExpandMoreRounded";
import WifiIcon from "@mui/icons-material/Wifi";
import BluetoothIcon from "@mui/icons-material/Bluetooth";
import SystemUpdateAltIcon from "@mui/icons-material/SystemUpdateAlt";
import SmartButtonTwoToneIcon from "@mui/icons-material/SmartButtonTwoTone";
import HassIO from "./icons/homeassistant.svg";
import AsyncAuto from "./AsynchronousAutoComplete";

const boardNames = [
  { title: "Custom PCB" },
  { title: "ESP32_Devkit_C" },
  { title: "Adafruit_Feather_Huzzah" },
  { title: "ESP32_Sketch_Board" },
  { title: "ESP32_DevKit_32s" },
  { title: "DOIT_ESP32" },
  { title: "ESP32_CAM" },
];

const firmwareVersions = [
  { title: "main" },
  { title: "nightly" },
  { title: "beta" },
  { title: "default build" },
];

const sht31Sensors = [{ title: "none" }, { title: "1" }, { title: "2" }];

const dhtSensorsType = [
  { title: "none" },
  { title: "dht11" },
  { title: "dht22" },
  { title: "dht21" },
  { title: "am2301" },
];

const lightSensors = [{ title: "none" }, { title: "bh1750" }, { title: "ldr" }];

const ds18b20 = [
  { title: "none" },
  { title: "1" },
  { title: "2" },
  { title: "3" },
  { title: "4" },
  { title: "5" },
  { title: "6" },
  { title: "7" },
  { title: "8" },
  { title: "9" },
  { title: "10" },
];

const dhtSensorsNum = ["none", "1", "2"];

const waterLevelSensors = [
  { title: "none" },
  { title: "infrared" },
  { title: "ultraSonic" },
  { title: "capacitive" },
];

const relayPin = [
  "32",
  "33",
]; /* Possible relay combinations - need logic to remove item from list if another sensor takes up that pin */

const GreenSwitch = styled(Switch)(({ theme }) => ({
  "& .MuiSwitch-switchBase.Mui-checked": {
    color: pink[600],
    "&:hover": {
      backgroundColor: alpha(pink[600], theme.palette.action.hoverOpacity),
    },
  },
  "& .MuiSwitch-switchBase.Mui-checked + .MuiSwitch-track": {
    backgroundColor: pink[600],
  },
}));

const AntSwitch = styled(Switch)(({ theme }) => ({
  width: 28,
  height: 16,
  padding: 0,
  display: "flex",
  "&:active": {
    "& .MuiSwitch-thumb": {
      width: 15,
    },
    "& .MuiSwitch-switchBase.Mui-checked": {
      transform: "translateX(9px)",
    },
  },
  "& .MuiSwitch-switchBase": {
    padding: 2,
    "&.Mui-checked": {
      transform: "translateX(12px)",
      color: "#fff",
      "& + .MuiSwitch-track": {
        opacity: 1,
        backgroundColor: theme.palette.mode === "dark" ? "#177ddc" : "#1890ff",
      },
    },
  },
  "& .MuiSwitch-thumb": {
    boxShadow: "0 2px 4px 0 rgb(0 35 11 / 20%)",
    width: 12,
    height: 12,
    borderRadius: 6,
    transition: theme.transitions.create(["width"], {
      duration: 200,
    }),
  },
  "& .MuiSwitch-track": {
    borderRadius: 16 / 2,
    opacity: 1,
    backgroundColor:
      theme.palette.mode === "dark"
        ? "rgba(255,255,255,.35)"
        : "rgba(0,0,0,.25)",
    boxSizing: "border-box",
  },
}));

/* DS18B20 */
const label = { inputProps: { "aria-label": "Switch demo" } };

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
  /* Board Names */
  const [value_boardNames, setValue_boardNames] = React.useState(boardNames[0]);
  const [inputValue_boardNames, setInputValue_boardNames] = React.useState("");

  /* Firmware Version */
  const [value_firmwareVersion, setValue_firmwareVersion] = React.useState(
    firmwareVersions[0]
  );
  const [inputValue_firmwareVersion, setInputValue_firmwareVersion] =
    React.useState("");

  /* BS18B20 */
  const [value_ds18b20, setValue_ds18b20] = React.useState(ds18b20[0]);
  const [inputValue_ds18b20, setInputValue_ds18b20] = React.useState("");

  /* DHT */
  const [value_dht, setValue_dht] = React.useState(dhtSensorsType[0]);
  const [inputValue_dht, setInputValue_dht] = React.useState("");

  /* light */
  const [value_light, setValue_light] = React.useState(lightSensors[0]);
  const [inputValue_light, setInputValue_light] = React.useState("");

  const [state, setState] = React.useState({
    mqtt: false,
    wifi: true,
    bluetooth: true,
    ota: false,
    mdns: false,
    hassio: false,
    light_sensor: false,
    sht31: false,
    ds18b20: false,
    dht: false,
    autoComplete_dht: false,
    autoComplete_ds18b20: false,
    autoComplete_light_sensor: false,
    autoComplete_water_level_sensor: false,
  });

  const handleChange = (event) => {
    setState({
      ...state,
      [event.target.name]: event.target.checked,
    });
  };

  const handleChangeAntSwitch = (event, type) => {
    setState({
      ...state,
      [type]: !state[type],
    });
  };

  const formSettings = () => {
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
                <ListItemText id="switch-list-label-mqtt" primary="MQTT" />
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
                <ListItemText id="switch-list-label-ota" primary="WiFi-OTA" />
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
      </div>
    );
  };

  const sensorOptions = () => {
    return (
      <div className="sensorOptions">
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
                className="sensorOptions-paper-left"
              >
                <Typography variant="h6" gutterBottom>
                  Humidity Sensors (Optional)
                </Typography>
                <FormLabel id="radio-buttons-group-label">
                  Enable Feature
                </FormLabel>
                <FormGroup>
                  <Stack direction="row" spacing={1} alignItems="center">
                    <Typography>Off</Typography>
                    <AntSwitch
                      name="dht"
                      defaultChecked
                      onChange={(event) =>
                        handleChangeAntSwitch(event, "autoComplete_dht")
                      }
                      id="antswitch-dht"
                    />
                    <Typography>On</Typography>
                  </Stack>
                </FormGroup>
                <br></br>
                <Tooltip
                  title={
                    <React.Fragment>
                      <p>
                        <b>{"DHT"}</b>{" "}
                        {"- What type of DHT sensor do you have?"}
                        <br></br>
                      </p>
                    </React.Fragment>
                  }
                  placement="top"
                  id="tooltip-top"
                >
                  {AsyncAuto(
                    [...dhtSensorsType],
                    "DHT Sensor Type",
                    false,
                    state.autoComplete_dht,
                    "dht"
                  )}
                </Tooltip>
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
                className="sensorOptions-paper-right"
              >
                <Typography variant="h6" gutterBottom>
                  Temperature Sensors (Optional)
                </Typography>
                <FormLabel id="radio-buttons-group-label">
                  Enable Feature
                </FormLabel>
                <FormGroup>
                  <Stack direction="row" spacing={1} alignItems="center">
                    <Typography>Off</Typography>
                    <AntSwitch
                      defaultChecked
                      name="ds18b20"
                      onChange={(event) =>
                        handleChangeAntSwitch(event, "autoComplete_ds18b20")
                      }
                      id="antswitch-ds18b20"
                    />
                    <Typography>On</Typography>
                  </Stack>
                </FormGroup>
                <br></br>
                <Tooltip
                  title={
                    <React.Fragment>
                      <p>
                        <b>{"DS18B20"}</b>{" "}
                        {"- How many sensors do you have?/want to use?"}
                        <br></br>
                      </p>
                    </React.Fragment>
                  }
                  placement="top"
                  id="tooltip-top"
                >
                  {AsyncAuto(
                    [...ds18b20],
                    "Number of DS18B20 Sensors",
                    false,
                    state.autoComplete_ds18b20,
                    "ds18b20"
                  )}
                </Tooltip>
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
                className="sensorOptions-paper-right"
              >
                <Typography variant="h6" gutterBottom>
                  Light Sensors
                </Typography>
                <FormLabel id="radio-buttons-group-label">
                  Enable Feature
                </FormLabel>
                <FormGroup>
                  <Stack direction="row" spacing={1} alignItems="center">
                    <Typography>Off</Typography>
                    <AntSwitch
                      defaultChecked
                      name="light"
                      onChange={(event) =>
                        handleChangeAntSwitch(event, "autoComplete_light_sensor")
                      }
                      id="antswitch-light"
                    />
                    <Typography>On</Typography>
                  </Stack>
                </FormGroup>
                <br></br>
                <Tooltip
                  title={
                    <React.Fragment>
                      <p>
                        <b>{"Light Sensors"}</b>{" "}
                        {"- What kind of sensor do you want to use?"}
                        <br></br>
                      </p>
                    </React.Fragment>
                  }
                  placement="top"
                  id="tooltip-top"
                >
                  {AsyncAuto(
                    [...lightSensors],
                    "Type of Light Level Sensor",
                    false,
                    state.autoComplete_light_sensor,
                    "light"
                  )}
                </Tooltip>
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
                className="sensorOptions-paper-right"
              >
                <Typography variant="h6" gutterBottom>
                  Water Level Sensors
                </Typography>
                <FormLabel id="radio-buttons-group-label">
                  Enable Feature
                </FormLabel>
                <FormGroup>
                  <Stack direction="row" spacing={1} alignItems="center">
                    <Typography>Off</Typography>
                    <AntSwitch
                      defaultChecked
                      name="water"
                      onChange={(event) =>
                        handleChangeAntSwitch(event, "autoComplete_water_level_sensor")
                      }
                      id="antswitch-water"
                    />
                    <Typography>On</Typography>
                  </Stack>
                </FormGroup>
                <br></br>
                <Tooltip
                  title={
                    <React.Fragment>
                      <p>
                        <b>{"Water Level"}</b>{" "}
                        {"- What kind of sensor do you want to use?"}
                        <br></br>
                      </p>
                    </React.Fragment>
                  }
                  placement="top"
                  id="tooltip-top"
                >
                  {AsyncAuto(
                    [...waterLevelSensors],
                    "Type of Water Level Sensor",
                    false,
                    state.autoComplete_water_level_sensor,
                    "water"
                  )}
                </Tooltip>
                <br></br>
              </Paper>
            </Grid>
          </Grid>
        </Grid>
      </div>
    );
  };

  return (
    <React.Fragment>
      <Typography variant="h6" gutterBottom>
        Versioning
      </Typography>
      <form>
        <Grid container spacing={3}>
          <Grid item sm={12}>
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
                <Tooltip title="Custom Name of the firmware" placement="top">
                  <TextField
                    id="firmwarename"
                    label="Firmware Name (Optional)"
                    fullWidth
                    autoComplete="firmwareName"
                  />
                </Tooltip>
                <br></br>
                {AsyncAuto(
                  [...firmwareVersions],
                  "Firmware Version",
                  true,
                  false,
                  "firmware_version"
                )}
                <br></br>
                <Tooltip
                  title={
                    <React.Fragment>
                      <p>
                        {"If unsure - choose"}{" "}
                        <strong>{"ESP32_Devkit_C"}</strong>
                        <br></br>
                        {
                          "If using the PCB design files from this repo, choose Custom PCB"
                        }
                      </p>
                    </React.Fragment>
                  }
                  placement="top"
                  id="tooltip-top"
                >
                  {AsyncAuto(
                    [...boardNames],
                    "Board Name",
                    true,
                    false,
                    "board_names"
                  )}
                </Tooltip>
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
                    <Typography>Configure Modules</Typography>
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
                    {formSettings()}
                  </AccordionDetails>
                </Accordion>
              </FormGroup>
            </FormControl>
          </Grid>
          {sensorOptions()}
        </Grid>
      </form>
    </React.Fragment>
  );
}
