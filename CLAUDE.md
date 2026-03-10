# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

A Zephyr RTOS application that displays elapsed time since a configured start date/time on an SSD1309 OLED display. Runs on a Seeed XIAO ESP32-C6 board with a PCF8563 RTC module over I2C. Time is synced via NTP over WiFi on boot. Designed for battery-only operation (no USB host required).

## Build & Flash

> **Note:** Building requires the Zephyr Python venv located one level up (`../zephyrproject/.venv` or similar). Do not attempt to run builds — ask the user to run them.

```shell
# Build (from repo root or app/ directory)
west build -b xiao_esp32c6/esp32c6/hpcore app

# Flash
west flash

# Run tests
west twister -T tests --integration
```

## Key Configuration Files

- **`app/include/start_time.h`** — Change the start date/time here. `tm_year` is years since 1900; `tm_mon` is 0-indexed (0=January).
- **`app/include/wifi_creds.h`** — WiFi credentials (not in git, must be created). Should define `known_networks[]` array and `KNOWN_NETWORKS_COUNT`.
- **`app/prj.conf`** — Kconfig settings for display (LVGL + SSD1309 monochrome), RTC, WiFi, NTP, and logging.
- **`app/boards/xiao_esp32c6_esp32c6_hpcore.overlay`** — Devicetree overlay: SSD1309 on SPI2, PCF8563 RTC on I2C0, sleep button on GPIO0.

## Architecture

The app has a single source file (`app/src/main.c`) with this flow:
1. Initialize display via LVGL — immediately shows elapsed time (from RTC) or "Syncing..."
2. Check RTC for existing time (PCF8563 retains time across deep sleep)
3. Configure sleep button (GPIO0 / D0 pin — wire button between D0 and GND)
4. Connect to WiFi (tries each entry in `known_networks[]`)
5. Sync RTC via SNTP (`time.cloudflare.com`)
6. Main loop (1 Hz): update display, auto-sleep after `AUTO_SLEEP_SECONDS` (default 3 min)

### Sleep / Wake

- **Sleep triggers**: button press on GPIO0, or auto-sleep after 3 minutes
- **Wake trigger**: button press on GPIO0 (deep-sleep GPIO wakeup)
- Deep sleep powers off the SoC; the external PCF8563 RTC keeps time
- On wake the device does a full reset: display shows time immediately from RTC, then NTP re-syncs in background

### Console / Logging

UART console is **disabled** (`CONFIG_UART_CONSOLE=n`) so the device boots without a USB host. To re-enable for debugging, set `CONFIG_UART_CONSOLE=y`, `CONFIG_LOG_BACKEND_UART=y`, `CONFIG_SERIAL=y`, and `CONFIG_CONSOLE=y` in `app/prj.conf`. Use `CONFIG_LOG_MODE_DEFERRED=y` (never IMMEDIATE) to avoid blocking on USB.

Elapsed time is computed as `(current RTC time) - START_TIME` in seconds. Falls back to `boot_time_seconds` counter if RTC is unavailable.

The repository structure includes scaffolding from the Zephyr example-application template (out-of-tree drivers, libs, custom west commands) — most of this scaffolding (`drivers/`, `lib/`, `scripts/`) is not used by the actual app logic.
