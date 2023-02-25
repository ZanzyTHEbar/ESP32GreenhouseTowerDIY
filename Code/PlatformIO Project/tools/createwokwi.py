#!/usr/bin/env python3

# https://docs.wokwi.com/vscode/getting-started

import toml
Import("env")


def createTOML(source, target, env):
    try:
        firmware_name = env.subst("$BUILD_DIR\${PROGNAME}")

        # find ".pio" in the firmware_name and split the string
        # to get the project name
        firmware_name = firmware_name.split(".pio")[1]

        # replace the leading "\" with a "/"

        firmware_name = firmware_name.replace("\\", "/")

        print("Creating wokwi.toml for %s" % firmware_name)

        wokwi_string = """\
        [wokwi]
        version = 1
        elf = ".pio{name}.elf"
        firmware = ".pio{name}.bin"
        """
        toml_string = wokwi_string.format(name=firmware_name)
        print(toml_string)

        parsed_toml = toml.loads(toml_string)

        new_toml_string = toml.dumps(parsed_toml)
        print(new_toml_string)

        with open("wokwi.toml", "w") as f:
            new_toml_string = toml.dump(parsed_toml, f)
        print("wokwi.toml created \n")

    except Exception as e:
        print("Error creating wokwi.toml: %s" % e)


env.AddPostAction("$BUILD_DIR\${PROGNAME}.bin", createTOML)
