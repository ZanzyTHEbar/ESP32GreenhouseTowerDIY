import { Button, Card, CardContent, Grid, Typography } from "@mui/material";
import { Box } from "@mui/system";

Object.defineProperty(exports, "__esModule", { value: true });
exports.ErrorPane = void 0;

export function ErrorPane(_a) {
  var error = _a.error;
  return (
    <Card variant="outlined">
      <CardContent>
        <Box sx={{ width: "100%" }}>
          <Typography
            mb={2}
            variant="h4"
            color="inherit"
            textAlign="center"
            noWrap
          >
            {error.title}
          </Typography>
          <Typography mb={2} variant="body1" color="inherit" textAlign="center">
            {error.message}
          </Typography>
          {error.action && error.actionText && (
            <Grid container justifyContent="center" alignItems="center">
              <Grid item xs={12} sm={4}>
                <Button
                  color="primary"
                  fullWidth
                  variant="contained"
                  onClick={error.action}
                >
                  {error.actionText}
                </Button>
              </Grid>
            </Grid>
          )}
        </Box>
      </CardContent>
    </Card>
  );
}
