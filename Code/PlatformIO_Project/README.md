# Wokwi Simulator

This project supports the [Wokwi](https://wokwi.com) simulator. It contains the following:

- `wokwi.toml` - the simulator configuration file
- `diagram.json` - the simulator parts config file
- `createwokwi.py` - a script to create a new `wokwi.toml` file every build
  - This is to make sure the simulator is always up to date with the latest version of the firmware

## Setup

`secrets.h`

You _must_ create a `secrets.h` file in the `include` directory. This file is used to store the `WIFI` credentials. The file should look like this:

```cpp
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""
```

If you are not using the simualtor, you can use your own `WIFI` credentials.

## How to use

In order to use the simulator, the password _must_ be blank and the ssid _must_ be `Wokwi-GUEST`. More information about this can be found [here](https://docs.wokwi.com/guides/esp32-wifi#connecting-to-the-wifi).

To use, it's pretty simple. Install the Wokwi VSCode Extension, and then open the project in VSCode. You can then use the Wokwi Simulator to simulate the project.

Press `F1` and type `Wokwi: Start Simulator`. This will start the simulator. You can then use the simulator to test the project.

To test the `API` navigate to `localhost:8180` in your browser. Then you can trigger the API endpoints.

Example:

```bash
localhost:8180/control/usercommands/command/helloWorld
```

This will trigger the `helloWorld` command, if you have not modified the `main.cpp`.

> **Note**: You will need to ahve a wokwi account to use the simulator. You will also need to create a new license key for the simulator. You can learn more about this [here](https://docs.wokwi.com/vscode/getting-started).

### Bug

There is a minor bug, for the time being. The `createwokwi.py` script will not crate a wokwi network, which is required for the simulator to work. To fix this, you will need to add the following to the `wokwi.toml` file after each build:

```toml
[[net.forward]]
from = "localhost:8180"
to = "target:80"
```

## Notes

The simulator does _not_ support `SPIFFS` at the moment. This means that you will not be able to load custom HTML files into the simulator. This is a limitation of the simulator, and not the project.
