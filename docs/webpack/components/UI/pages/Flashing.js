import * as React from "react";
import * as espWebTools from "esp-web-tools";
import { Button, Grid } from "@mui/material/";

export default function Flashing() {
  return (
    <div>
      <Grid container spacing={3}>
        <Grid item sm={12}></Grid>
      </Grid>
      <esp-web-install-button manifest="assets/manifest.json">
        <button slot="activate">Upload Code</button>
        <span slot="unsupported">
          Your current browser is not supported. Please use Chrome for the best
          experience.
        </span>
        <span slot="not-allowed">
          HTTPS is required for this feature. Please use a browser that supports
          HTTPS.
        </span>
      </esp-web-install-button>
    </div>
  );
}
