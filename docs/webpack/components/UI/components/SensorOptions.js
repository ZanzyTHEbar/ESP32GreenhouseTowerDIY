import React from "react";
import {
  Grid,
  Paper,
  Tooltip,
  FormLabel,
  FormGroup,
  Stack,
  Typography,
  Switch
} from "@mui/material";
import AsyncAuto from "../components/AsynchronousAutoComplete";
import { alpha, styled } from "@mui/material/styles";

/*********************************************************************************************************
 *
 * Variables and Constants
 *
 ********************************************************************************************************* */

/* Form Constants */
const numberofHumiditySensors = [{ title: "1" }, { title: "2" }];

const humiditySensorsType = [
  { title: "dht11" },
  { title: "dht22" },
  { title: "dht21" },
  { title: "am2301" },
  { title: "sht31" }
];

const lightSensors = [{ title: "bh1750" }, { title: "ldr" }];

const ds18b20 = [
  { title: "1" },
  { title: "2" },
  { title: "3" },
  { title: "4" },
  { title: "5" },
  { title: "6" },
  { title: "7" },
  { title: "8" },
  { title: "9" },
  { title: "10" }
];

const waterLevelSensors = [
  { title: "infrared" },
  { title: "ultraSonic" },
  { title: "capacitive" }
];

const humSensorsNum = [{ title: "1" }, { title: "2" }];

const relayPin = [
  { title: "32" },
  { title: "33" }
]; /* Possible relay combinations - need logic to remove item from list if another sensor takes up that pin */

const AntSwitch = styled(Switch)(({ theme }) => ({
  width: 28,
  height: 16,
  padding: 0,
  display: "flex",
  "&:active": {
    "& .MuiSwitch-thumb": {
      width: 15
    },
    "& .MuiSwitch-switchBase.Mui-checked": {
      transform: "translateX(9px)"
    }
  },
  "& .MuiSwitch-switchBase": {
    padding: 2,
    "&.Mui-checked": {
      transform: "translateX(12px)",
      color: "#fff",
      "& + .MuiSwitch-track": {
        opacity: 1,
        backgroundColor: theme.palette.mode === "dark" ? "#177ddc" : "#1890ff"
      }
    }
  },
  "& .MuiSwitch-thumb": {
    boxShadow: "0 2px 4px 0 rgb(0 35 11 / 20%)",
    width: 12,
    height: 12,
    borderRadius: 6,
    transition: theme.transitions.create(["width"], {
      duration: 200
    })
  },
  "& .MuiSwitch-track": {
    borderRadius: 16 / 2,
    opacity: 1,
    backgroundColor:
      theme.palette.mode === "dark"
        ? "rgba(255,255,255,.35)"
        : "rgba(0,0,0,.25)",
    boxSizing: "border-box"
  }
}));

const SensorOptions = () => {
  const [state, setState] = React.useState({
    autoComplete_hum: false,
    autoComplete_ds18b20: false,
    autoComplete_light_sensor: false,
    autoComplete_water_level_sensor: false
  });

  const handleChangeAntSwitch = (event, type) => {
    setState({
      ...state,
      [type]: !state[type]
    });
  };

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
                bgcolor: "background.paper"
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
                    name="hum"
                    defaultChecked
                    onChange={(event) =>
                      handleChangeAntSwitch(event, "autoComplete_hum")
                    }
                    id="antswitch-hum"
                  />
                  <Typography>On</Typography>
                </Stack>
              </FormGroup>
              <br></br>
              <Tooltip
                title={
                  <React.Fragment>
                    <p>
                      <b>{"Humidity"}</b> {"- What type of sensor do you have?"}
                      <br></br>
                    </p>
                  </React.Fragment>
                }
                placement="top"
                id="tooltip-top"
              >
                {AsyncAuto(
                  [...humiditySensorsType],
                  "Humidity Sensor Type",
                  false,
                  state.autoComplete_hum,
                  "hum"
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
                bgcolor: "background.paper"
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
                bgcolor: "background.paper"
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
                bgcolor: "background.paper"
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

export default SensorOptions;
