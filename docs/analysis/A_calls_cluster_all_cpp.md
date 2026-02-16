| File A | File B | Number of call sites | Direction |
|---|---|---:|---|
| src/audio/Audio.cpp | src/audio/mp3_decoder/mp3_decoder.cpp | 4 | A→B |
| src/audio/Audio.cpp | src/src/audio_handler.cpp | 53 | A→B |
| src/main.cpp | src/src/adc_handler.cpp | 2 | A→B |
| src/main.cpp | src/src/audio_handler.cpp | 85 | bidirectional |
| src/main.cpp | src/src/button_handler.cpp | 3 | A→B |
| src/main.cpp | src/src/dtmf_handler.cpp | 2 | A→B |
| src/main.cpp | src/src/file_handler.cpp | 5 | A→B |
| src/main.cpp | src/src/gpio_handler.cpp | 46 | A→B |
| src/main.cpp | src/src/imu_handler.cpp | 2 | A→B |
| src/main.cpp | src/src/leddisplay_handler.cpp | 5 | A→B |
| src/main.cpp | src/src/modem_handler.cpp | 7 | A→B |
| src/main.cpp | src/src/power_handler.cpp | 9 | A→B |
| src/main.cpp | src/src/test_handler.cpp | 1 | A→B |
| src/main.cpp | src/src/tft_handler.cpp | 16 | A→B |
| src/main.cpp | src/src/tick.cpp | 84 | A→B |
| src/main.cpp | src/src/wifi_handler.cpp | 2 | A→B |
| src/src/audio_handler.cpp | src/src/dtmf_handler.cpp | 10 | B→A |
| src/src/audio_handler.cpp | src/src/test_handler.cpp | 12 | B→A |
| src/src/audio_handler.cpp | src/src/tick.cpp | 9 | A→B |
| src/src/button_handler.cpp | src/src/file_handler.cpp | 2 | A→B |
| src/src/button_handler.cpp | src/src/tick.cpp | 3 | A→B |
| src/src/common.cpp | src/src/file_handler.cpp | 1 | B→A |
| src/src/common.cpp | src/src/modem_handler.cpp | 47 | B→A |
| src/src/common.cpp | src/src/wifi_handler.cpp | 1 | B→A |
| src/src/dtmf_handler.cpp | src/src/gpio_handler.cpp | 1 | A→B |
| src/src/dtmf_handler.cpp | src/src/tick.cpp | 2 | A→B |
| src/src/file_handler.cpp | src/src/modem_handler.cpp | 1 | B→A |
| src/src/file_handler.cpp | src/src/wifi_handler.cpp | 1 | B→A |
| src/src/gpio_handler.cpp | src/src/power_handler.cpp | 4 | B→A |
| src/src/gpio_handler.cpp | src/src/test_handler.cpp | 5 | B→A |
| src/src/gpio_handler.cpp | src/src/tick.cpp | 18 | A→B |
| src/src/imu_handler.cpp | src/src/tick.cpp | 2 | A→B |
| src/src/leddisplay_handler.cpp | src/src/tick.cpp | 2 | A→B |
| src/src/modem_handler.cpp | src/src/serial_transport.cpp | 1 | A→B |
| src/src/modem_handler.cpp | src/src/test_handler.cpp | 3 | B→A |
| src/src/modem_handler.cpp | src/src/tick.cpp | 36 | A→B |
| src/src/power_handler.cpp | src/src/tick.cpp | 4 | A→B |

Callback/function-pointer pass relationships between files:
 none observed
