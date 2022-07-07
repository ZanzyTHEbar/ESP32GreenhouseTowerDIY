import React from "react";
import {
  Grid,
  Typography,
  InputLabel,
  InputAdornment,
  OutlinedInput,
  Box,
  Paper,
  IconButton,
  AppBar,
  Alert,
  AlertTitle,
  FormControl,
  FormGroup,
  TextField,
  Tooltip,
} from "@mui/material";
import WifiIcon from "@mui/icons-material/Wifi";
import { Visibility, VisibilityOff } from "@mui/icons-material";

const WifiSection = () => {
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
                        WiFi network. If you do not configure this section, the
                        device will default to using the access point details
                        configured in the software. See the{" "}
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
                            name="ssid"
                            placeholder="ESP32_Greenhouse_Tower"
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
                              placeholder="plantplant"
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

export default WifiSection;
