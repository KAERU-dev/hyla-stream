# Hyla Stream

Hyla Stream is the KAERU fork of
[moonlight-qt](https://github.com/moonlight-stream/moonlight-qt) that
[Hyla Launcher](https://github.com/KAERU-dev/hyla-launcher) starts for its
**Stream** option. "Hyla Stream" is a working name; the product name is settled
before the first distributed build. Hyla runs it as a separate GPL-3.0
executable through the command line below and never links it.

Hyla Stream keeps moonlight-qt's GPL-3.0 license and credits; moonlight-qt's own
README follows below. Everything not listed here is unchanged so upstream
changes merge cleanly.

## Changes from moonlight-qt

- `--data-dir` keeps settings, the pairing identity, caches and logs in one
  directory.
- `--hyla` prints JSON status lines and exits with an error code instead of
  showing dialogs, for the `stream` and `pair` actions.
- A `discover` action lists hosts found over mDNS.
- The update check against moonlight-stream.org is gone; Hyla updates this
  client.
- The organization and application names are `KAERU` and `Hyla Stream`, so
  even without `--data-dir` it does not share files with an installed
  Moonlight. Binary names, the desktop file ID and the icon are unchanged for
  now.
- CI also builds against the system Qt 6 on Ubuntu 24.04, for Hyla's deb
  package.

## Command-line contract

Hyla depends on this contract; change it only together with Hyla's
integration.

### Options for every action

| Option             | Meaning                                                                                                   |
| ------------------ | --------------------------------------------------------------------------------------------------------- |
| `--data-dir <dir>` | Store settings (`<dir>/KAERU/Hyla Stream.ini`), caches (`<dir>/cache`) and logs (`<dir>/logs`) in `<dir>` |
| `--hyla`           | Print JSON status lines on stdout and exit with the codes below instead of showing dialogs                |

The `HYLA_STREAM_DATA_DIR` environment variable sets the data directory when
`--data-dir` is absent. Hyla passes the same data directory to every call so
pairing and streaming share one identity. Qt may still keep its own shader
cache under the standard `KAERU/Hyla Stream` cache location.

### Actions

```
moonlight [options] pair <host> [--pin <4 digits>]
moonlight [options] stream <host> <app> [stream options, see stream --help]
moonlight [options] quit <host>
moonlight [options] list <host> [--csv]
moonlight [options] discover [--json] [--timeout <1-60 seconds, default 5>]
```

`quit` and `list` behave as in moonlight-qt.

### Status lines (`--hyla`)

Each line on stdout is one JSON object with a `status` field:

| Status      | Action | Extra fields                    | Meaning                                       |
| ----------- | ------ | ------------------------------- | --------------------------------------------- |
| `starting`  | stream | `host`, `app`                   | Looking for the host and app                  |
| `streaming` | stream | —                               | The connection started and video is showing   |
| `ended`     | stream | —                               | The stream ended without an error; exit `0`   |
| `pairing`   | pair   | `host`, `pin`                   | Enter `pin` on the host                       |
| `paired`    | pair   | —                               | Pairing succeeded; exit `0`                   |
| `error`     | both   | `code`, `message` (translated)  | The action failed; exit with the code below   |

Logs go to stderr and the log file, never to stdout.

### Exit codes

| Code | `error.code`       | Meaning                                                    |
| ---- | ------------------ | ---------------------------------------------------------- |
| 0    | —                  | Success                                                    |
| 1    | —                  | Invalid arguments (message on stderr, no status line)      |
| 10   | `host_unreachable` | The host did not answer in time                            |
| 11   | `not_paired`       | Streaming from a host that is not paired                   |
| 12   | `already_paired`   | Pairing with a host that is already paired                 |
| 13   | `app_not_found`    | The host has no app with that name                         |
| 14   | `app_running`      | Another app is running on the host; quit it first          |
| 15   | `pairing_failed`   | The host rejected pairing or the PIN was wrong             |
| 16   | `stream_failed`    | The stream failed to start or ended with an error          |

### `discover --json`

Prints one JSON document after the timeout:

```json
{"hosts":[{"hostname":"desktop.local","port":47989,"addresses":["192.168.1.14","fe80::7866:d3cd:92a2:3fde"]}]}
```

Discovered hosts are not saved. Pair with one of its addresses.

---

# Moonlight PC

[Moonlight PC](https://moonlight-stream.org) is an open source PC client for NVIDIA GameStream and [Sunshine](https://github.com/LizardByte/Sunshine).

Moonlight also has mobile versions for [Android](https://github.com/moonlight-stream/moonlight-android) and [iOS](https://github.com/moonlight-stream/moonlight-ios).

You can follow development on our [Discord server](https://moonlight-stream.org/discord) and help translate Moonlight into your language on [Weblate](https://hosted.weblate.org/projects/moonlight/moonlight-qt/).

 [![Build](https://img.shields.io/github/actions/workflow/status/moonlight-stream/moonlight-qt/build.yml?branch=master)](https://github.com/moonlight-stream/moonlight-qt/actions/workflows/build.yml?query=branch%3Amaster)
 [![Downloads](https://img.shields.io/github/downloads/moonlight-stream/moonlight-qt/total)](https://github.com/moonlight-stream/moonlight-qt/releases)
 [![Translation Status](https://hosted.weblate.org/widgets/moonlight/-/moonlight-qt/svg-badge.svg)](https://hosted.weblate.org/projects/moonlight/moonlight-qt/)

## Features
 - Hardware accelerated video decoding on Windows, Mac, and Linux
 - H.264, HEVC, and AV1 codec support (AV1 requires Sunshine and a supported host GPU)
 - YUV 4:4:4 support (Sunshine only)
 - HDR streaming support
 - 7.1 surround sound audio support
 - 10-point multitouch support (Sunshine only)
 - Gamepad support with force feedback and motion controls for up to 16 players
 - Support for both pointer capture (for games) and direct mouse control (for remote desktop)
 - Support for passing system-wide keyboard shortcuts like Alt+Tab to the host
 
## Downloads
- [Windows, macOS, and Steam Link](https://github.com/moonlight-stream/moonlight-qt/releases)
- [Snap (for Ubuntu-based Linux distros)](https://snapcraft.io/moonlight)
- [Flatpak (for other Linux distros)](https://flathub.org/apps/details/com.moonlight_stream.Moonlight)
- [AppImage](https://github.com/moonlight-stream/moonlight-qt/releases)
- [Raspberry Pi 4 and 5](https://github.com/moonlight-stream/moonlight-docs/wiki/Installing-Moonlight-Qt-on-Raspberry-Pi-4)
- [Generic ARM 32-bit and 64-bit Debian packages](https://github.com/moonlight-stream/moonlight-docs/wiki/Installing-Moonlight-Qt-on-ARM%E2%80%90based-Single-Board-Computers) (not for Raspberry Pi)
- [Experimental RISC-V Debian packages](https://github.com/moonlight-stream/moonlight-docs/wiki/Installing-Moonlight-Qt-on-RISC%E2%80%90V-Single-Board-Computers)
- [NVIDIA Jetson and Nintendo Switch (Ubuntu L4T)](https://github.com/moonlight-stream/moonlight-docs/wiki/Installing-Moonlight-Qt-on-Linux4Tegra-(L4T)-Ubuntu)

### Nightly Builds
- [Downloads](https://nightly.link/moonlight-stream/moonlight-qt/workflows/build/master)

#### Special Thanks

[![Hosted By: Cloudsmith](https://img.shields.io/badge/OSS%20hosting%20by-cloudsmith-blue?logo=cloudsmith&style=flat-square)](https://cloudsmith.com)

Hosting for Moonlight's Debian and L4T package repositories is graciously provided for free by [Cloudsmith](https://cloudsmith.com).

## Building

### Windows Build Requirements
* Qt 6.11 SDK or later (earlier versions may work but are not officially supported)
* [Visual Studio 2026](https://visualstudio.microsoft.com/downloads/) (Community edition is fine)
* Select **MSVC** option during Qt installation. MinGW is not supported.
* [7-Zip](https://www.7-zip.org/) (only if building installers for non-development PCs)
* Graphics Tools (only if running debug builds)
  * Install "Graphics Tools" in the Optional Features page of the Windows Settings app.
  * Alternatively, run `dism /online /add-capability /capabilityname:Tools.Graphics.DirectX~~~~0.0.1.0` and reboot.

### macOS Build Requirements
* Qt 6.11 SDK or later (earlier versions may work but are not officially supported)
* Xcode 15 or later (earlier versions may work but are not officially supported)
* [create-dmg](https://github.com/sindresorhus/create-dmg) (only if building DMGs for use on non-development Macs)

### Linux/Unix Build Requirements
* Qt 6 is recommended, but Qt 5.12 or later is also supported (replace `qmake6` with `qmake` when using Qt 5).
* GCC or Clang
* FFmpeg 4.0 or later
* Install the required packages:
  * Debian/Ubuntu:
    * Base Requirements: `libegl1-mesa-dev libgl1-mesa-dev libopus-dev libsdl2-dev libsdl2-ttf-dev libssl-dev libavcodec-dev libavformat-dev libswscale-dev libva-dev libvdpau-dev libxkbcommon-dev wayland-protocols libdrm-dev`
    * Qt 6 (Recommended): `qt6-base-dev qt6-declarative-dev libqt6svg6-dev qt6-wayland qml6-module-qtquick-controls qml6-module-qtquick-templates qml6-module-qtquick-layouts qml6-module-qtqml-workerscript qml6-module-qtquick-window qml6-module-qtquick`
    * Qt 5: `qtbase5-dev qt5-qmake qtdeclarative5-dev qtquickcontrols2-5-dev qml-module-qtquick-controls2 qml-module-qtquick-layouts qml-module-qtquick-window2 qml-module-qtquick2 qtwayland5`
  * RedHat/Fedora (RPM Fusion repo required):
    * Base Requirements: `openssl-devel SDL2-devel SDL2_ttf-devel ffmpeg-devel libva-devel libvdpau-devel opus-devel pulseaudio-libs-devel alsa-lib-devel libdrm-devel`
    * Qt 6 (Recommended): `qt6-qtsvg-devel qt6-qtdeclarative-devel`
    * Qt 5: `qt5-qtsvg-devel qt5-qtquickcontrols2-devel`
* Building the Vulkan renderer requires a `libplacebo-dev`/`libplacebo-devel` version of at least v7.349.0 and FFmpeg 6.1 or later.

### Steam Link Build Requirements
* [Steam Link SDK](https://github.com/ValveSoftware/steamlink-sdk) cloned on your build system
* STEAMLINK_SDK_PATH environment variable set to the Steam Link SDK path

**Steam Link Hardware Limitations**  
Moonlight builds for Steam Link are subject to hardware limitations of the Steam Link device:
* Maximum resolution: **1080p (1920x1080)**
* Maximum framerate: **60 FPS**
* Maximum video bitrate: **40 Mbps**
* **HDR streaming is not supported** on the original hardware

### Docker containers
If you want to use Docker for building, look at [this repo](https://github.com/cgutman/moonlight-packaging) containing canonical containers
for different architectures, which handle building deps and extra linking for you.

### Build Setup Steps
1. Install the latest Qt SDK (and optionally, the Qt Creator IDE) from https://www.qt.io/download
    * You can install Qt via Homebrew on macOS, but you will need to use `brew install qt --with-debug` to be able to create debug builds of Moonlight.
    * You may also use your Linux distro's package manager for the Qt SDK as long as the packages are Qt 5.12 or later.
    * This step is not required for building on Steam Link, because the Steam Link SDK includes Qt 5.14.
2. Download submodules and dependencies
    * Run `git submodule update --init --recursive` from within `moonlight-qt/`.
    * On Windows and macOS, you must also run `setup-deps.ps1` (Windows) or `setup-deps.py` (macOS).
    * Perform these steps each time you pull new changes from the Git repository.
3. Open the project in Qt Creator or build from qmake on the command line.
    * To build a binary for use on non-development machines, use the scripts in the `scripts` folder.
        * For Windows builds, use `scripts\build-arch.bat` and `scripts\generate-bundle.bat`. Execute these scripts from the root of the repository within a Qt command prompt. Ensure  7-Zip binary directory is on your `%PATH%`.
        * For macOS builds, use `scripts/generate-dmg.sh`. Execute this script from the root of the repository and ensure Qt's `bin` folder is in your `$PATH`.
        * For Steam Link builds, run `scripts/build-steamlink-app.sh` from the root of the repository.
    * To build from the command line for development use on macOS or Linux, run `qmake6 moonlight-qt.pro` then `make debug` or `make release`.
        * The final binary will be placed in `app/moonlight`.
    * To create an embedded build for a single-purpose device, use `qmake6 "CONFIG+=embedded" moonlight-qt.pro` and build normally.
        * This build will lack windowed mode, Discord/Help links, and other features that don't make sense on an embedded device.
        * For platforms with poor GPU performance, add `"CONFIG+=gpuslow"` to prefer direct KMSDRM rendering over GL/Vulkan renderers. Direct KMSDRM rendering can use dedicated YUV/RGB conversion and scaling hardware rather than slower GPU shaders for these operations.

## Contribute
1. Fork us
2. Write code
3. Send Pull Requests

Check out our [website](https://moonlight-stream.org) for project links and information.
