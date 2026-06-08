# time-since

A small battery-powered "time since" counter. It shows the elapsed time since a
fixed start date/time on an OLED display and counts up second by second — handy
for "days since X". Built on [Zephyr RTOS](https://www.zephyrproject.org/).

```
 000042
 13 07 21
   ^elapsed: 42 days, 13 hours, 07 minutes, 21 seconds
```

## What it does

1. Boots and immediately shows the elapsed time read from a battery-backed RTC.
2. Connects to WiFi and corrects the clock over NTP on every boot.
3. Counts up once per second on the display.
4. Sleeps to save battery: press the button, or after 3 minutes of auto-sleep.
5. Wakes instantly on a button press; the external RTC keeps time while asleep, so
   the count stays correct across sleep cycles.

The elapsed time is computed in UTC (`current_epoch − start_epoch`), so it is
correct regardless of timezone. The configured start instant is interpreted as
**Berlin local time** (DST-aware) and converted to UTC internally.

## Hardware

| Part            | Detail                                                        |
|-----------------|---------------------------------------------------------------|
| Board           | Seeed Studio XIAO ESP32-C6 (`xiao_esp32c6/esp32c6/hpcore`)     |
| Display         | SSD1309 128×64 monochrome OLED, on **SPI2**                    |
| RTC             | NXP PCF8563 real-time clock, on **I2C0** (addr `0x51`)         |
| Sleep button    | Momentary button between **D0 (GPIO0)** and **GND**           |
| Power           | Battery only — no USB host required                           |

Exact pin mapping lives in the devicetree overlay
`app/boards/xiao_esp32c6_esp32c6_hpcore.overlay` (SPI/I2C pins, reset and
data/command GPIOs, button alias `sw0`). Wire the button directly from D0 to GND;
the pin uses an internal pull-up and is active-low.

## Repository layout

```
app/
  src/            main, gui (LVGL), wifi/NTP, power/deep-sleep, time math, fonts
  include/        start_time.h, wifi_creds.h (you create), secrets.h.example, headers
  boards/         devicetree overlay for the XIAO ESP32-C6
  prj.conf        Kconfig: display, RTC, WiFi, NTP, logging, power management
tests/app/        ztest suites (time_utils: epoch/DST/format)
west.yml          west manifest — imports Zephyr + only the modules this app needs
```

This repo is the **west manifest repository**: `west.yml` pins Zephyr and the
modules the app needs (`hal_espressif`, `lvgl`, `mbedtls`, `tf-psa-crypto`).

## Prerequisites (Ubuntu/Debian)

Install the host build tools (see the
[Zephyr getting-started guide](https://docs.zephyrproject.org/latest/develop/getting_started/)
for other OSes):

```shell
sudo apt install --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget python3-dev python3-venv python3-tk \
  xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1
```

## 1. Create the workspace

The app lives **next to** Zephyr inside a west workspace. Clone this repo, then
let west initialise the workspace around it:

```shell
# Workspace folder (any name/location works)
mkdir time-since-workspace && cd time-since-workspace

# This repo = the west manifest
git clone https://github.com/DonPavlov/time-since

# Python virtualenv for west + Zephyr tooling (activate it every session)
python3 -m venv .venv
source .venv/bin/activate
pip install west

# Initialise the workspace from the manifest, then pull Zephyr + modules
west init -l time-since
west update

# Register Zephyr's CMake package and install its Python deps
west zephyr-export
west packages pip --install
```

After this the workspace looks like:

```
time-since-workspace/
  .venv/        Python virtualenv
  .west/        west config
  zephyr/       Zephyr RTOS (cloned by west update)
  modules/      hal_espressif, lvgl, mbedtls, tf-psa-crypto
  time-since/   this app
```

## 2. Install the toolchain and ESP32 blobs

```shell
# Zephyr SDK (compilers — includes the riscv64 toolchain the ESP32-C6 uses)
west sdk install

# Espressif binary blobs (WiFi/PHY firmware) — required to link the WiFi stack
west blobs fetch hal_espressif
```

## 3. Configure

**WiFi credentials** — create `app/include/wifi_creds.h` (gitignored). It must
define the `struct wifi_network` type, the `known_networks[]` array, and
`KNOWN_NETWORKS_COUNT`. Use `app/include/secrets.h.example` as a template:

```c
static const struct wifi_network known_networks[] = {
        {
                .ssid = "YourSSID",
                .password = "YourPassword",
                .security = WIFI_SECURITY_TYPE_PSK,
        },
};
#define KNOWN_NETWORKS_COUNT (sizeof(known_networks) / sizeof(known_networks[0]))
```

The app tries each entry in order until one connects.

**Start date/time** — edit `app/include/start_time.h`. Fields follow `struct
rtc_time`: `tm_year` is **years since 1900** (2025 → `125`) and `tm_mon` is
**0-indexed** (0 = January). The value is treated as Berlin local time.

```c
static const struct rtc_time START_TIME = {
        .tm_year = 125,   /* 2025 */
        .tm_mon  = 4,     /* May  */
        .tm_mday = 15,
        .tm_hour = 22,
        .tm_min  = 0,
        .tm_sec  = 0,
};
```

## 4. Build & flash

From inside the `time-since` directory (venv active):

```shell
cd time-since
west build -b xiao_esp32c6/esp32c6/hpcore app

# Flash over USB
west flash
```

Add `-p always` to `west build` for a pristine (clean) rebuild.

## 5. Run the tests (no hardware needed)

The unit tests build and run on the host simulator (`native_sim`):

```shell
west twister -T tests -p native_sim
```

The `time_utils` suite covers UTC epoch conversion, Berlin DST handling, and the
elapsed-time string formatting.

## Usage & power behaviour

- **Wake:** press the button — the display lights up instantly.
- **Sleep:** a short press shuts the device down. Sleep is only armed after the
  device has been on for **3 seconds**, so a long press or a "hold to make sure it
  boots" will not immediately power it off again.
- **Auto-sleep:** after 3 minutes the device sleeps on its own.
- During deep sleep the SoC is powered off; the PCF8563 RTC keeps time on its own
  supply, so the count resumes correctly on the next wake.
- The serial console is intentionally disabled (`CONFIG_SERIAL=n`); logging runs in
  deferred mode with no UART backend, because enabling the console on this board
  freezes the device.

## Troubleshooting

- **Display stuck at `000000`** — no valid time yet. The RTC reports a clock-
  integrity failure when it has never been set (cold power-up), so the app shows
  zero until NTP succeeds. Check WiFi credentials and that the network is reachable.
- **Wrong elapsed time** — verify `start_time.h` (remember `tm_year` is since 1900
  and `tm_mon` is 0-indexed) and that the start instant is the intended Berlin
  local time.
- **`undefined symbol LV_*` / `MBEDTLS` / `TF-PSA-Crypto` Kconfig or CMake errors**
  — a required module is missing. Re-run `west update` so `west.yml`'s module list
  (`hal_espressif`, `lvgl`, `mbedtls`, `tf-psa-crypto`) is fully cloned.
- **Link error about `esp_..._gpio_wakeup`** — Espressif HAL API drift; the deep-
  sleep wakeup call lives in `app/src/power.c`.
