import * as React from "react";
import {
  Button,
  Grid,
} from "@mui/material/";

function DownloadButton() {
  return (
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
  );
}

export default function Downloading() {
  return (
    <div>
      <DownloadButton />
    </div>
  );
}
