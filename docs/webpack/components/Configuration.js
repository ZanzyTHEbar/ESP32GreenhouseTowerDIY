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
  InputAdornment,
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
import { Visibility, VisibilityOff } from "@mui/icons-material";
import HassIO from "./icons/homeassistant.svg";
import AsyncAuto from "./AsynchronousAutoComplete";
import "./style.css";
import Styled, { keyframes } from "styled-components";
/* import Animations from "./animation"; */
/* import { IconName } from "react-icons/fa";
https://react-icons.github.io/react-icons/icons?name=fa */

/*********************************************************************************************************
 *
 * Variables and Constants
 *
 ********************************************************************************************************* */

/* Form Constants */
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

const waterLevelSensors = [
  { title: "none" },
  { title: "infrared" },
  { title: "ultraSonic" },
  { title: "capacitive" },
];

const dhtSensorsNum = [{ title: "none" }, { title: "1" }, { title: "2" }];

const relayPin = [
  { title: "32" },
  { title: "33" },
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

/*********************************************************************************************************
 *
 * Functions
 *
 ********************************************************************************************************* */

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

const Configuration = ({
  handleNext,
  handleChange,
  values: { firmwareName, ssid, password },
  formErrors,
}) => {
  // Check if all values are not empty or if there are some error
  const isValid =
    firmwareName.length > 0 &&
    !formErrors.firmwareName &&
    ssid.length > 0 &&
    !formErrors.ssid &&
    password.length > 0 &&
    !formErrors.password;

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

  const handleButtonChange = (event) => {
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
                        handleChangeAntSwitch(
                          event,
                          "autoComplete_light_sensor"
                        )
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
                        handleChangeAntSwitch(
                          event,
                          "autoComplete_water_level_sensor"
                        )
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

  const wifiSection = () => {
    const [values, setValues] = React.useState({
      password: "",
      showPassword: false,
    });

    const handleButtonChange = (prop) => (event) => {
      setValues({ ...values, [prop]: event.target.value });
    };

    const handleClickShowPassword = () => {
      setValues({
        ...values,
        showPassword: !values.showPassword,
      });
    };

    const handleMouseDownPassword = (event) => {
      event.preventDefault();
    };
    return (
      <div className="wifiSection">
        <Box
          sx={{
            display: "flex",
            width: "100%",
            maxWidth: 800,
            alignItems: "center",
            justifyContent: "center",
            bgcolor: "background.paper",
          }}
        >
          <React.Fragment>
            <Paper
              variant="elevated"
              sx={{
                margin: "1rem",
                padding: "1rem",
                width: "100%",
                maxWidth: 800,
                bgcolor: "background.paper",
              }}
              elevation={3}
            >
              <AppBar position="relative">
                <Typography
                  align="center"
                  variant="h3"
                  gutterBottom
                  color="inherit"
                >
                  <WifiIcon style={{ position: "relative", top: "3px" }} />{" "}
                  Settings
                </Typography>
              </AppBar>
              <Grid container spacing={3}>
                <Grid item sm={12}>
                  <Paper
                    variant="outlined"
                    sx={{
                      padding: "1rem",
                      marginTop: "1rem",
                      width: "100%",
                      bgcolor: "background.paper",
                    }}
                    className="wifiSection-paper"
                  >
                    <Typography variant="h6" gutterBottom>
                      Access Point Settings
                    </Typography>
                    <Grid item sm={12}>
                      <FormControl
                        component="fieldset"
                        variant="outlined"
                        required
                        fullWidth
                      >
                        <Alert
                          severity="warning"
                          role="info"
                          variant="filled"
                          style={{ background: "#02cf86" }}
                        >
                          <AlertTitle>Info</AlertTitle>
                          We will{" "}
                          <u>
                            <strong>never</strong>
                          </u>{" "}
                          collect, save, or otherwise interact with your WiFi
                          credentials.
                        </Alert>
                        <Typography variant="h6" gutterBottom>
                          The below fields are for configuring the Access Point
                          that will be used to connect your device to your local
                          WiFi network. If you do not configure this section,
                          the device will default to using the access point
                          details configured in the software. See the{" "}
                          <a href="/ESP32GreenhouseTowerDIY/access_point">
                            Access Point
                          </a>{" "}
                          section of the documentation for more information.
                        </Typography>
                        <br></br>
                        <FormGroup>
                          <Tooltip
                            title="Custom SSID of the Access Point"
                            placement="top"
                          >
                            <TextField
                              id="greenhouse-ssid"
                              label="SSID (Optional)"
                              value={ssid || ""}
                              name="ssid"
                              onChange={handleChange}
                              error={!!formErrors.ssid}
                              helperText={formErrors.ssid}
                              fullWidth
                            />
                          </Tooltip>
                          <br></br>
                          <Tooltip
                            title="Custom Password of the Access Point"
                            placement="top"
                          >
                            <FormControl variant="outlined">
                              <InputLabel htmlFor="outlined-adornment-password">
                                Password (Optional)
                              </InputLabel>
                              <OutlinedInput
                                id="outlined-adornment-password"
                                type={values.showPassword ? "text" : "password"}
                                value={values.password}
                                onChange={handleButtonChange("password")}
                                endAdornment={
                                  <InputAdornment position="end">
                                    <IconButton
                                      aria-label="toggle password visibility"
                                      onClick={handleClickShowPassword}
                                      onMouseDown={handleMouseDownPassword}
                                      edge="end"
                                    >
                                      {values.showPassword ? (
                                        <VisibilityOff />
                                      ) : (
                                        <Visibility />
                                      )}
                                    </IconButton>
                                  </InputAdornment>
                                }
                                label="Password (Optional)"
                              />
                            </FormControl>
                          </Tooltip>
                        </FormGroup>
                      </FormControl>
                    </Grid>
                  </Paper>
                </Grid>
              </Grid>
            </Paper>
          </React.Fragment>
        </Box>
      </div>
    );
  };

  return (
    <React.Fragment>
      <Typography variant="h6" gutterBottom>
        Versioning
      </Typography>
      <Grid container spacing={3}>
        <Grid item sm={12}>
          <FormControl
            component="fieldset"
            variant="outlined"
            required
            fullWidth
          >
            <Alert severity="error" role="info" variant="filled" color="error">
              <AlertTitle>Warning</AlertTitle>
              It is recommended to use the <strong>most recent</strong> version
            </Alert>
            <br></br>
            <FormGroup>
              <Tooltip title="Custom Name of the firmware" placement="top">
                <TextField
                  id="firmwarename"
                  label="Firmware Name (Optional)"
                  value={firmwareName || ""}
                  placeholder="my_greenhouse_firmware"
                  onChange={handleChange}
                  name="firmwareName"
                  error={!!formErrors.firmwareName}
                  helperText={formErrors.firmwareName}
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
                      {"If unsure - choose"} <strong>{"ESP32_Devkit_C"}</strong>
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
                    Customize your Build Configuration with the below settings.
                    Certain Features require the wifi module to be enabled. This
                    project uses LAN for communication, so you will need to
                    enable the wifi module. If you really do not want to enable
                    the wifi module, you can disable it here, and bluetooth with
                    be enabled automatically.
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
      {wifiSection()}
    </React.Fragment>
  );
};

export default Configuration;
