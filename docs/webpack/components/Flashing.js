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

function DownloadButton() {
  return (
    <form>
      <Grid container spacing={3}>
        <Grid item sm={12}>
          <Button
            variant="contained"
            color="primary"
            fullWidth
            onClick={() => {
              console.log(
                `${value_boardNames} and firmware version ${value_firmwareVersion}`
              );
            }}
          >
            Download
          </Button>
        </Grid>
      </Grid>
    </form>
  );
}

export default function Downloading() {
  return (
    <div>
      <DownloadButton />
    </div>
  );
}
