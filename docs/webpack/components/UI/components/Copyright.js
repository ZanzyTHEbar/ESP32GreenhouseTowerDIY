import { Link, Typography } from "@mui/material";

export function Copyright() {
  const year = new Date().getFullYear();
  const name = "ZanzyTHEbar";
  const url = "https://zanzythebar.github.io/ESP32GreenhouseTowerDIY/";
  const message = "ESP32GreenhouseTowerDIY";
  return (
    <Typography variant="body2" color="text.secondary" my={1} align="center">
      {"Copyright © "}
      <Link color="inherit" href={url}>
        {message}
      </Link>{" "}
      {year}
      {". Made with love by "}
      <Link color="inherit" href={`https://github.com/${name}/`}>
        {name}
      </Link>
    </Typography>
  );
}
