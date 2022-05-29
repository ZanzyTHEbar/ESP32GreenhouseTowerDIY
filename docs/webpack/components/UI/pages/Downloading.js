import * as React from "react";
import {
  Button,
  Box,
  Grid,
  LinearProgress,
} from "@mui/material/";

function LinearBuffer() {
  const [progress, setProgress] = React.useState(0);
  const [buffer, setBuffer] = React.useState(10);

  const progressRef = React.useRef(() => {});
  React.useEffect(() => {
    progressRef.current = () => {
      if (progress > 100) {
        setProgress(0);
        setBuffer(10);
      } else {
        const diff = Math.random() * 10;
        const diff2 = Math.random() * 10;
        setProgress(progress + diff);
        setBuffer(progress + diff + diff2);
      }
    };
  });

  React.useEffect(() => {
    const timer = setInterval(() => {
      progressRef.current();
    }, 500);

    return () => {
      clearInterval(timer);
    };
  }, []);

  return (
    <Box sx={{ width: "100%" }}>
      <LinearProgress variant="buffer" value={progress} valueBuffer={buffer} />
    </Box>
  );
}

function LinearIndeterminate() {
  return (
    <Box sx={{ width: "100%" }}>
      <LinearProgress />
    </Box>
  );
}

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
      <LinearIndeterminate />
      <br></br>
      <DownloadButton />
    </div>
  );
}
