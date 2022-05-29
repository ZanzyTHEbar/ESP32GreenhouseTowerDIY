import { InfoOutlined } from "@mui/icons-material";
import { Grid, Link } from "@mui/material";

export function HelperComponent({link, text,}) {
  return (
    <Grid container mt={1} flexDirection="row" alignContent="center">
      <Grid item mx={0.5}>
        <InfoOutlined sx={{ verticalAlign: "middle" }} fontSize="small" />
      </Grid>
      <Grid item justifySelf="center">
        <Link
          sx={{ fontSize: 12 }}
          href={link}
          target="_blank"
          rel="noreferrer"
        >
          {text}
        </Link>
      </Grid>
    </Grid>
  );
}
