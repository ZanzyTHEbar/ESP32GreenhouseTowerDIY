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
  values: { firstName, lastName, email, gender, date, phone, city },
}) => {
  const handleSubmit = () => {
    console.log({ firstName, lastName, email, gender, date, phone, city });
    handleNext();
  };
  return (
    <div>
      <Grid container spacing={3}>
        <Grid item sm={12}>
          <React.Fragment>
            <List disablePadding>
              <ListItem>
                <ListItemText primary="First Name" secondary={firstName} />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText primary="Last Name" secondary={lastName} />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText primary="Email Address" secondary={email} />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText primary="Gender" secondary={gender} />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText primary="Date of birth" secondary={date} />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText primary="City" secondary={city} />
              </ListItem>

              <Divider />

              <ListItem>
                <ListItemText
                  primary="phone"
                  secondary={phone.length > 0 ? phone : "Not Provided"}
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
