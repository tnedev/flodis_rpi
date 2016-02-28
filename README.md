Flow

The code was written using Atom and the PlatformIO package.

Install Atom:
  https://atom.io/docs/v0.194.0/getting-started-installing-atom

Install PlatformIO:
  Atom -> Edit -> Preferences -> Install -> "platfomio-ide", "platform-ide-terminal"

Install PlatformIO Arduino libraries:
  MsTimer2: terminal "platformio lib install 137"
  OneWire: terminal "platformio lib install 1"

Problem with gcc-linter package:
  The gcc-linter can't find the platformio libraries and frequently shows errors with them.
  gcc-linter package could be disabled.
