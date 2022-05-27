import * as React from "react";
import {
  Button,
  Divider,
  List,
  ListItemText,
  ListItem,
  Grid,
} from "@mui/material/";

const Done = ({
  handleNext,
  handleBack,
  values: { firmwareName, firmwareVersion, boardName, ssid, password },
}) => {
  const handleSubmit = () => {
    console.log({ firmwareName, firmwareVersion, boardName, ssid, password });
    handleNext();
  };
  return (
    <div>
      <Grid container spacing={3}>
        <Grid item sm={12}>
          <React.Fragment>
            <List disablePadding>
              <ListItem>
                <ListItemText
                  primary="Custom Firmware Name"
                  secondary={firmwareName === null ? "Not Set" : firmwareName}
                />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText
                  primary="Firmware Version"
                  secondary={firmwareVersion}
                />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText
                  primary="Board Name"
                  secondary={boardName}
                />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText
                  primary="Access Point SSID"
                  secondary={ssid === null ? "ESP32_Greenhouse_Tower" : ssid}
                />
              </ListItem>
            </List>
          </React.Fragment>
        </Grid>
      </Grid>
    </div>
  );
};

export default Done;
