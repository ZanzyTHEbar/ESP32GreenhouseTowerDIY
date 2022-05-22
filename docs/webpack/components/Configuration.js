import * as React from "react";
import {
  Button,
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
  Stepper,
  Step,
  StepLabel,
  StepContent,
  Link,
  StepConnector,
  StepConnectorClasses,
} from "@mui/material/";

const boardNames = ["ESP32_Devkit_C", "Adafruit_Feather_32"];
const firmwareVersions = ["main", "nightly", "beta"];

function DownloadButton(){
  return (
    <form>
      <Grid container spacing={3}>
        <Grid item sm={12}>
          <Button
            variant="contained"
            color="primary"
            fullWidth
            onClick={() => {
              console.log(`${value_boardNames} and firmware version ${value_firmwareVersion}`);
            }}
          >
            Download
          </Button>
        </Grid>
      </Grid>
    </form>
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
    gilad: true,
    jason: false,
    antoine: true,
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
            <FormControl component="fieldset" variant="standard" fullWidth>
              <FormLabel component="legend">
                It is recommended to use the most recent version
              </FormLabel>
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
                  id="controllable-states-demo"
                  options={firmwareVersions}
                  fullWidth
                  renderInput={(params) => (
                    <TextField {...params} label="Firmware version" />
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
                  id="controllable-states-demo"
                  options={boardNames}
                  fullWidth
                  renderInput={(params) => (
                    <TextField {...params} label="Board Name" />
                  )}
                />
                <FormHelperText>If unsure - choose ESP32_Devkit_C</FormHelperText>
                {/* <FormControlLabel
                  control={
                    <Switch
                      checked={state.gilad}
                      onChange={handleChange}
                      name="gilad"
                    />
                  }
                  label="Gilad Gray"
                />
                <FormControlLabel
                  control={
                    <Switch
                      checked={state.jason}
                      onChange={handleChange}
                      name="jason"
                    />
                  }
                  label="Jason Killian"
                />
                <FormControlLabel
                  control={
                    <Switch
                      checked={state.antoine}
                      onChange={handleChange}
                      name="antoine"
                    />
                  }
                  label="Antoine Llorca"
                /> */}
              </FormGroup>
            </FormControl>
          </Grid>
        </Grid>
      </form>
      
    </React.Fragment>
  );
}
