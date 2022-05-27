import React, { Component, StrictMode } from "react";
import {
  Button,
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
import SaveIcon from "@mui/icons-material/SaveAltTwoTone";
import DeleteIcon from "@mui/icons-material/Delete";
import { createTheme, ThemeProvider } from "@mui/material/styles";
import Configuration from "./Configuration";
import Building from "./Building";
import Downloading from "./Downloading";
import Flashing from "./Flashing";
import Done from "./Done";
import Success from "./Success";
import formValidation from "./helpers/formValidation";
import "./style.css";

const initialValues = {
  firmwareName: "",
  firmwareVersion: "",
  boardName: "",
  ssid: "",
  password: "",
};

const fieldsValidation = {
  firmwareName: { error: "", validate: "text", minLength: 2, maxLength: 20 },
  firmwareVersion: { error: "", validate: "checkbox" },
  boardName: { error: "", validate: "checkbox" },
  ssid: { error: "", validate: "text", minLength: 2, maxLength: 20 },
  password: { error: "", validate: "text", minLength: 2, maxLength: 20 },
};

const steps = [
  {
    label: "Configuration",
    description: `This section is used to configure the ESP32 firmware image. Please choose the functionality you want to enable.`,
  },
  {
    label: "Building",
    description: ``,
  },
  {
    label: "Downloading",
    description: ``,
  },
  {
    label: "Flashing",
    description: ``,
  },
  {
    label: "Done",
    description: ``,
  },
];

const theme = createTheme({
  status: {
    danger: "#e53e3e",
  },
  palette: {
    primary: {
      main: "#0971f1",
      darker: "#053e85",
    },
    neutral: {
      main: "#64748B",
      contrastText: "#fff",
    },
  },
  components: {
    // Name of the component
    MuiButtonBase: {
      defaultProps: {},
      styleOverrides: {
        root: ({ ownerState }) => ({
          ...(ownerState.variant === "contained" &&
            ownerState.color === "primary" && {
              backgroundColor: "#202020",
              color: "#fff",
            }),
        }),
      },
    },
  },
});

const MUI = () => {
  const [activeStep, setActiveStep] = React.useState(0);
  const [skipped, setSkipped] = React.useState(new Set());
  const [formValues, setFormValues] = React.useState(initialValues);
  const [formErrors, setFormErrors] = React.useState({});

  const isStepOptional = (step) => {
    return step === 2;
  };

  const isStepSkipped = (step) => {
    return skipped.has(step);
  };

  const handleNext = () => {
    let newSkipped = skipped;
    if (isStepSkipped(activeStep)) {
      newSkipped = new Set(newSkipped.values());
      newSkipped.delete(activeStep);
    }

    setActiveStep((prevActiveStep) => prevActiveStep + 1);
    setSkipped(newSkipped);
  };

  const handleBack = () => {
    setActiveStep((prevActiveStep) => prevActiveStep - 1);
  };

  const handleSkip = () => {
    if (!isStepOptional(activeStep)) {
      // You probably want to guard against something like this,
      // it should never occur unless someone's actively trying to break something.
      throw new Error("You can't skip a step that isn't optional.");
    }

    setActiveStep((prevActiveStep) => prevActiveStep + 1);
    setSkipped((prevSkipped) => {
      const newSkipped = new Set(prevSkipped.values());
      newSkipped.add(activeStep);
      return newSkipped;
    });
  };

  const handleReset = () => {
    setActiveStep(0);
  };

  const handleFlashAnother = () => {
    setActiveStep(0);
    setFormValues(initialValues);
    setFormErrors({});
  };

  // Handle form change
  const handleChange = (e) => {
    const { name, value } = e.target;

    // Set values
    setFormValues((prev) => ({
      ...prev,
      [name]: value,
    }));

    // set errors
    const error = formValidation(name, value, fieldsValidation) || "";

    setFormErrors({
      [name]: error,
    });
  };

  const handleSteps = (step) => {
    switch (step) {
      case 0:
        return (
          <Configuration
            handleNext={handleNext}
            handleChange={handleChange}
            values={formValues}
            formErrors={formErrors}
          />
        );
      case 1:
        return (
          <Building
            handleNext={handleNext}
            handleChange={handleChange}
            values={formValues}
            formErrors={formErrors}
          />
        );
      case 2:
        return (
          <Downloading
            handleNext={handleNext}
            handleChange={handleChange}
            values={formValues}
            formErrors={formErrors}
          />
        );
      case 3:
        return (
          <Flashing
            handleNext={handleNext}
            handleChange={handleChange}
            values={formValues}
            formErrors={formErrors}
          />
        );
      case 4:
        return (
          <Done
            handleNext={handleNext}
            handleChange={handleChange}
            values={formValues}
            formErrors={formErrors}
          />
        );
      default:
        throw new Error("Unknown step");
    }
  };

  return (
    <div className="react-mui-ui">
      <>
        {activeStep === steps.length ? (
          // Last Component
          <Success handleNext={handleFlashAnother} handleReset={handleReset} />
        ) : (
          <>
            <ThemeProvider theme={theme}>
              <CssBaseline enableColorScheme={true} />
              <main>
                <Container maxWidth="lg" className="react-mui-ui-container">
                  <Paper variant="elevation" elevation={0}>
                    {/* <Alert severity="info" role="alert" variant="filled" color="info">
                This is an info alert — check it out!
              </Alert> */}
                  </Paper>
                  <Paper variant="elevation" elevation={0}>
                    <AppBar position="relative" color="primary">
                      <Typography align="center" variant="h2" color="inherit">
                        Configure your Firmware Image
                      </Typography>
                    </AppBar>
                    <Box sx={{ maxWidth: 800 }}>
                      <Stepper activeStep={activeStep} orientation="vertical">
                        {steps.map((step, index) => {
                          const stepProps = {};
                          const labelProps = {};
                          if (isStepOptional(index)) {
                            labelProps.optional = (
                              <Typography variant="caption">
                                Optional
                              </Typography>
                            );
                          }
                          if (isStepSkipped(index)) {
                            stepProps.completed = false;
                          }
                          return (
                            <Step key={step.label}>
                              <StepLabel
                                optional={
                                  index === 4 ? (
                                    <Typography variant="caption">
                                      Last step
                                    </Typography>
                                  ) : null
                                }
                              >
                                {step.label}
                              </StepLabel>
                              <StepContent>
                                <Typography>{step.description}</Typography>
                                <Box sx={{ mb: 2 }}>
                                  <>
                                    {activeStep === steps.length ? (
                                      <React.Fragment>
                                        <Typography sx={{ mt: 2, mb: 1 }}>
                                          All steps completed - you&apos;re
                                          finished
                                        </Typography>
                                        <Box
                                          sx={{
                                            display: "flex",
                                            flexDirection: "row",
                                            pt: 2,
                                          }}
                                        >
                                          <Box sx={{ flex: "1 1 auto" }} />
                                          <Button onClick={handleReset}>
                                            Reset
                                          </Button>
                                        </Box>
                                      </React.Fragment>
                                    ) : (
                                      <React.Fragment>
                                        <React.Fragment>
                                          {handleSteps(activeStep)}
                                          <Box
                                            sx={{
                                              display: "flex",
                                              justifyContent: "flex-end",
                                              flex: "1 1 auto",
                                            }}
                                          >
                                            {activeStep !== 0 && (
                                              <Button
                                                onClick={handleBack}
                                                sx={{ mt: 3, ml: 1 }}
                                              >
                                                Back
                                              </Button>
                                            )}
                                            {isStepOptional(activeStep) && (
                                              <Button
                                                onClick={handleSkip}
                                                sx={{ mt: 3, ml: 1 }}
                                              >
                                                Skip
                                              </Button>
                                            )}
                                            <Button
                                              variant="contained"
                                              onClick={handleNext}
                                              sx={{ mt: 3, ml: 1 }}
                                              color={
                                                activeStep === steps.length - 1
                                                  ? "secondary"
                                                  : "primary"
                                              }
                                            >
                                              {activeStep === steps.length - 1
                                                ? "Finish"
                                                : "Next"}
                                            </Button>
                                          </Box>
                                        </React.Fragment>
                                      </React.Fragment>
                                    )}
                                  </>
                                </Box>
                              </StepContent>
                            </Step>
                          );
                        })}
                      </Stepper>
                      {activeStep === steps.length && (
                        <Paper square elevation={0} sx={{ p: 3 }}>
                          <Typography>
                            All steps completed - you&apos;re finished
                          </Typography>
                          <Button onClick={handleReset} sx={{ mt: 1, mr: 1 }}>
                            Reset
                          </Button>
                        </Paper>
                      )}
                    </Box>
                  </Paper>
                </Container>
              </main>
            </ThemeProvider>
          </>
        )}
      </>
    </div>
  );
};

export default MUI;
