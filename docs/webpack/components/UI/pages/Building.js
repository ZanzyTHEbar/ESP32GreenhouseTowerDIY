import * as React from "react";
import {
  Button,
  Grid,
} from "@mui/material/";

function showMsg() {
  const msg = "Setting up Firmware Image";
  window.alert(msg);
  return msg;
}

function DownloadButton() {
  return (
     
      <Grid container spacing={3}>
        <Grid item sm={12}>
          <Button
            variant="contained"
            color="primary"
            fullWidth
            onClick={() => {showMsg();}}
          >
            Build Firmware
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
