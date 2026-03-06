# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Is

A Zephyr RTOS application that displays elapsed time since a configured start date/time on an SSD1309 OLED display. Runs on a Seeed XIAO ESP32-C6 board with a PCF8563 RTC module over I2C. Time is synced via NTP over WiFi on boot and re-checked every 5 minutes.

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
- **`app/boards/xiao_esp32c6_esp32c6_hpcore.overlay`** — Devicetree overlay: SSD1309 on SPI2, PCF8563 RTC on I2C0.

## Architecture

The app has a single source file (`app/src/main.c`) with this flow:
1. Initialize display via LVGL
2. Check RTC for existing time
3. Connect to WiFi (tries each entry in `known_networks[]`)
4. Sync RTC via SNTP (`pool.ntp.org`)
5. Main loop (1 Hz): calculate elapsed seconds from `START_TIME`, update display labels, re-sync RTC if drift >5s (checked every 5 min)

Elapsed time is computed as `(current RTC time) - START_TIME` in seconds, with day rollover handling. Falls back to `boot_time_seconds` counter if RTC is unavailable.

The repository structure includes scaffolding from the Zephyr example-application template (out-of-tree drivers, libs, custom west commands) — most of this scaffolding (`drivers/`, `lib/`, `scripts/`) is not used by the actual app logic.
