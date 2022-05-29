import * as React from "react";
import {
  Accordion,
  AccordionSummary,
  AccordionDetails,
  FormGroup,
  FormControl,
  TextField,
  Typography,
  Grid,
  Alert,
  AlertTitle,
  Tooltip,
} from "@mui/material/";
import ExpandMoreIcon from "@mui/icons-material/ExpandMoreRounded";
import FormSettings from "../components/FormSettings";
import SensorOptions from "../components/SensorOptions";
import WifiSection from "../components/WifiSection";
import AsyncAuto from "../components/AsynchronousAutoComplete";
import "../../style.css";
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

/* DS18B20 */
const label = { inputProps: { "aria-label": "Switch demo" } };

const Configuration = () => {
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
                  placeholder="my_greenhouse_firmware"
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
                  <FormSettings />
                </AccordionDetails>
              </Accordion>
            </FormGroup>
          </FormControl>
        </Grid>
        <SensorOptions />
      </Grid>
      <WifiSection />
    </React.Fragment>
  );
};

export default Configuration;
