# Interaction Clusters (Fan-out Weighted)

## Cluster 1
Included files: src/audio/Audio.cpp, src/audio/mp3_decoder/mp3_decoder.cpp, src/main.cpp, src/src/adc_handler.cpp, src/src/audio_handler.cpp, src/src/button_handler.cpp, src/src/common.cpp, src/src/dtmf_handler.cpp, src/src/file_handler.cpp, src/src/gpio_handler.cpp, src/src/imu_handler.cpp, src/src/leddisplay_handler.cpp, src/src/modem_handler.cpp, src/src/power_handler.cpp, src/src/serial_transport.cpp, src/src/test_handler.cpp, src/src/tft_handler.cpp, src/src/tick.cpp, src/src/wifi_handler.cpp

Total internal score: 2374.259417

Top 5 strongest internal edges with evidence:

| file_i | file_j | total_score | evidence |
|---|---|---:|---|
| src/main.cpp | src/src/audio_handler.cpp | 274.9519284 | state:generalFlags.audio_test_abort_request(k=2,m=0.63); state:generalFlags.audio_test_request(k=2,m=0.63); state:statusReg.flags.audio_digit_playing(k=2,m=0.63); calls:85 dir:bidirectional |
| src/main.cpp | src/src/modem_handler.cpp | 248.5011419 | state:generalFlags.audio_test_cleanup_request(k=2,m=0.63); state:generalFlags.call_answer_request(k=2,m=0.63); state:generalFlags.call_audio_request(k=2,m=0.63); ctx:loop; calls:7 dir:A→B |
| src/src/file_handler.cpp | src/src/wifi_handler.cpp | 203.7016103 | state:configReg.UIN.val(k=2,m=0.63); state:configReg.flags.Val(k=2,m=0.63); state:configReg.whitelist_inherit1(k=2,m=0.63); ctx:http route handler; calls:1 dir:B→A |
| src/src/modem_handler.cpp | src/src/wifi_handler.cpp | 192.8577502 | state:configReg.UIN.lval(k=2,m=0.63); state:configReg.flags.ftp_secure_enabled(k=2,m=0.63); state:generalFlags.sim_switch_reboot_request(k=2,m=0.63); ctx:loop |
| src/main.cpp | src/src/tick.cpp | 168 | calls:84 dir:A→B |

## Bridge files

### 1. src/main.cpp
Weighted degree: 1202.569626

Top 3 strongest neighbour edges with evidence:

| neighbour | total_score | evidence |
|---|---:|---|
| src/src/audio_handler.cpp | 274.9519284 | state:generalFlags.audio_test_abort_request(k=2,m=0.63); state:generalFlags.audio_test_request(k=2,m=0.63); state:statusReg.flags.audio_digit_playing(k=2,m=0.63); calls:85 dir:bidirectional |
| src/src/modem_handler.cpp | 248.5011419 | state:generalFlags.audio_test_cleanup_request(k=2,m=0.63); state:generalFlags.call_answer_request(k=2,m=0.63); state:generalFlags.call_audio_request(k=2,m=0.63); ctx:loop; calls:7 dir:A→B |
| src/src/tick.cpp | 168 | calls:84 dir:A→B |

### 2. src/src/modem_handler.cpp
Weighted degree: 825.9899271

Top 3 strongest neighbour edges with evidence:

| neighbour | total_score | evidence |
|---|---:|---|
| src/main.cpp | 248.5011419 | state:generalFlags.audio_test_cleanup_request(k=2,m=0.63); state:generalFlags.call_answer_request(k=2,m=0.63); state:generalFlags.call_audio_request(k=2,m=0.63); ctx:loop; calls:7 dir:A→B |
| src/src/wifi_handler.cpp | 192.8577502 | state:configReg.UIN.lval(k=2,m=0.63); state:configReg.flags.ftp_secure_enabled(k=2,m=0.63); state:generalFlags.sim_switch_reboot_request(k=2,m=0.63); ctx:loop |
| src/src/file_handler.cpp | 140.5045238 | state:configReg.UIN.byte(k=3,m=0.50); state:configReg.flags.location_text_setup(k=3,m=0.50); state:configReg.ftp_directory(k=3,m=0.50); ctx:task: modem; ctx:loop; calls:1 dir:B→A |

### 3. src/src/wifi_handler.cpp
Weighted degree: 579.3872703

Top 3 strongest neighbour edges with evidence:

| neighbour | total_score | evidence |
|---|---:|---|
| src/src/file_handler.cpp | 203.7016103 | state:configReg.UIN.val(k=2,m=0.63); state:configReg.flags.Val(k=2,m=0.63); state:configReg.whitelist_inherit1(k=2,m=0.63); ctx:http route handler; calls:1 dir:B→A |
| src/src/modem_handler.cpp | 192.8577502 | state:configReg.UIN.lval(k=2,m=0.63); state:configReg.flags.ftp_secure_enabled(k=2,m=0.63); state:generalFlags.sim_switch_reboot_request(k=2,m=0.63); ctx:loop |
| src/main.cpp | 123.6410583 | state:configReg.flags.audio_path_test_enable(k=2,m=0.63); state:configReg.flags.no_network_response_enabled(k=2,m=0.63); state:statusReg.call_status(k=2,m=0.63); ctx:loop; calls:2 dir:A→B |

### 4. src/src/file_handler.cpp
Weighted degree: 472.2508928

Top 3 strongest neighbour edges with evidence:

| neighbour | total_score | evidence |
|---|---:|---|
| src/src/wifi_handler.cpp | 203.7016103 | state:configReg.UIN.val(k=2,m=0.63); state:configReg.flags.Val(k=2,m=0.63); state:configReg.whitelist_inherit1(k=2,m=0.63); ctx:http route handler; calls:1 dir:B→A |
| src/src/modem_handler.cpp | 140.5045238 | state:configReg.UIN.byte(k=3,m=0.50); state:configReg.flags.location_text_setup(k=3,m=0.50); state:configReg.ftp_directory(k=3,m=0.50); ctx:task: modem; ctx:loop; calls:1 dir:B→A |
| src/main.cpp | 90.89099262 | state:configReg.audo_path_test_period(k=3,m=0.50); state:configReg.call_disable_low_signal_level(k=3,m=0.50); state:configReg.flags.external_power_disable_enabled(k=3,m=0.50); ctx:loop; calls:5 dir:A→B |

### 5. src/src/audio_handler.cpp
Weighted degree: 455.6185951

Top 3 strongest neighbour edges with evidence:

| neighbour | total_score | evidence |
|---|---:|---|
| src/main.cpp | 274.9519284 | state:generalFlags.audio_test_abort_request(k=2,m=0.63); state:generalFlags.audio_test_request(k=2,m=0.63); state:statusReg.flags.audio_digit_playing(k=2,m=0.63); calls:85 dir:bidirectional |
| src/audio/Audio.cpp | 106 | calls:53 dir:A→B |
| src/src/test_handler.cpp | 24 | calls:12 dir:B→A |

