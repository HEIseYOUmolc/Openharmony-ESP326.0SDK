# Troubleshooting

## Build fails with missing IDF components

- Verify `IDF_PATH` points to ESP-IDF v6.0.1.
- Run `idf.py reconfigure`.

## Wi-Fi demo never gets IP

- Confirm SSID and password in menuconfig.
- Check region and AP compatibility.

## Demo output missing

- Ensure exactly one `OHOS_DEMO_*` choice is selected.
- Confirm serial monitor baud rate is 115200.

## Watchdog reset on boot

- Increase `CONFIG_ESP_MAIN_TASK_STACK_SIZE`.
- Disable unsupported demo while validating kernel startup.
