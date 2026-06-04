# Target Onboarding Contract

For each new `idf.py set-target <idf_target>` chip, add the following:

1. `components/ohos_port/src/targets/<idf_target>/target_port.c`
2. `components/ohos_port/src/targets/<idf_target>/Kconfig.target`
3. `components/ohos_port/src/targets/<idf_target>/sdkconfig.defaults`
4. A board profile yaml in `board_profile/`
5. A source-selection entry in `components/ohos_port/CMakeLists.txt`

`target_port.c` must provide:
- `void ohos_port_target_init(void)`
- `const ohos_port_board_profile_t *ohos_port_board_profile_get(void)`
