import React from "react";
import { Box, Typography, Button, Tooltip } from "@mui/material/";

const Success = ({ handleNext, handleReset }) => {
  const handleSubmit = () => {
    handleNext();
  };
  const handleResetForm = () => {
    handleReset();
  };
  return (
    <Box>
      <Typography variant="h2" align="center">
        Thank you!
      </Typography>
      <Typography component="p" align="center">
        Enjoy your new Vertical Tower Garden!
      </Typography>
      <div
        style={{
          display: "flex",
          marginTop: 50,
          justifyContent: "center",
          alignContent: "center",
        }}
      >
        <Tooltip title="You can always reset the form to start over.">
          <Button variant="contained" color="primary" onClick={handleResetForm}>
            Reset Form
          </Button>
        </Tooltip>
        <br></br>
        <Tooltip
          title={
            <React.Fragment>
              This flashes another board with the{" "}
              <u>
                <strong>same settings</strong>
              </u>
              .
            </React.Fragment>
          }
        >
          <Button
            style={{ marginLeft: 20 }}
            variant="contained"
            color="secondary"
            onClick={handleSubmit}
          >
            Flash Another Garden Tower
          </Button>
        </Tooltip>
        <br></br>
      </div>
    </Box>
  );
};

export default Success;
// Language: javascript
// Path: docs\webpack\components\Success.js
