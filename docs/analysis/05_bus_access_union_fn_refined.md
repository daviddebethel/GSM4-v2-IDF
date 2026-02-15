| Hub | Field path | File | Enclosing function | Execution context | Unknown reason | R | W | Notes |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| configReg | configReg.PIN | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  | Y |  | checked |
| configReg | configReg.PIN | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.PIN | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.PIN | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.PIN | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.PIN | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.PIN | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.PIN | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.PIN | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.PIN | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.PIN | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.PIN | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.UIN.byte | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.UIN.byte | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.UIN.byte | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.UIN.byte | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.UIN.lval | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.UIN.lval | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.UIN.lval | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.UIN.val | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.UIN.val | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.UIN.val | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.UIN.val | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.UIN.val | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.announcement_volume | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.announcement_volume | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.announcement_volume | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.announcement_volume | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.announcement_volume | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.announcement_volume | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.announcement_volume | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.announcement_volume | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.announcement_volume | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.announcement_volume | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.announcement_volume | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.announcement_volume | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.announcement_volume | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.announcement_volume | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.audo_path_test_period | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.audo_path_test_period | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.audo_path_test_period | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.audo_path_test_period | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.audo_path_test_period | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.audo_path_test_period | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.audo_path_test_period | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.audo_path_test_period | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.audo_path_test_period | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.audo_path_test_period | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.audo_path_test_period | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.auto_answer_rings | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.auto_answer_rings | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.auto_answer_rings | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.auto_answer_rings | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.auto_answer_rings | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_answer_rings | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_answer_rings | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_answer_rings | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.auto_answer_rings | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.auto_answer_rings | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.auto_answer_rings | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.auto_answer_rings | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.auto_answer_rings | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.auto_answer_rings | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.auto_dial_time | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.auto_dial_time | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.auto_dial_time | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.auto_dial_time | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.auto_dial_time | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_dial_time | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_dial_time | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_dial_time | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.auto_dial_time | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.auto_dial_time | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.auto_dial_time | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.auto_dial_time | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.auto_dial_time | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.auto_onhook_time | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.auto_onhook_time | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.auto_onhook_time | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.auto_onhook_time | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.auto_onhook_time | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_onhook_time | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_onhook_time | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.auto_onhook_time | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.auto_onhook_time | src/src/modem_handler.cpp | sms_build_inftext | task: modem |  | Y |  | checked |
| configReg | configReg.auto_onhook_time | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.auto_onhook_time | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.auto_onhook_time | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.auto_onhook_time | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.auto_onhook_time | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.auto_onhook_time | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.battery_chargerate | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.battery_chargerate | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.battery_chargerate | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.battery_chargerate | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.battery_chargerate | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.battery_chargerate | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.battery_chargerate | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.battery_chargerate | src/src/power_handler.cpp | power_task | loop |  | Y |  | checked |
| configReg | configReg.battery_chargerate | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.battery_chargerate | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.battery_chargerate | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.battery_chargerate | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.battery_low_level | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.battery_low_level | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.battery_low_level | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.battery_low_level | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.battery_low_level | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.battery_low_level | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.battery_low_level | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.battery_low_level | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.battery_low_level | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.battery_low_level | src/src/power_handler.cpp | power_task | loop |  | Y |  | checked |
| configReg | configReg.battery_low_level | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.battery_low_level | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.battery_low_level | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.battery_low_level | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.call_disable_low_signal_level | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.call_disable_low_signal_level | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.call_disable_low_signal_level | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.call_disable_low_signal_level | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.call_disable_low_signal_level | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.call_disable_low_signal_level | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.call_disable_low_signal_level | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.call_disable_low_signal_level | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.call_disable_low_signal_level | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.call_disable_low_signal_level | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.call_disable_low_signal_level | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.call_log_min_duration | src/main.cpp | log_task | loop |  | Y |  | checked |
| configReg | configReg.call_log_min_duration | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.call_log_min_duration | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.call_log_min_duration | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.call_log_min_duration | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.call_log_min_duration | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.call_log_min_duration | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.call_log_min_duration | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.call_log_min_duration | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.call_log_min_duration | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.call_log_min_duration | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.call_log_min_duration | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.door_illumination_timeout | src/main.cpp | door_illumination_process | loop |  | Y |  | checked |
| configReg | configReg.door_illumination_timeout | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.door_illumination_timeout | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.door_illumination_timeout | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.door_illumination_timeout | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.door_illumination_timeout | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.door_illumination_timeout | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.door_illumination_timeout | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.door_illumination_timeout | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.door_illumination_timeout | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.door_illumination_timeout | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.door_illumination_timeout | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.door_illumination_timeout | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.door_illumination_timeout | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.door_led_brightness | src/main.cpp | door_illumination_process | loop |  | Y |  | checked |
| configReg | configReg.door_led_brightness | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.door_led_brightness | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.door_led_brightness | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.door_led_brightness | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.door_led_brightness | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.door_led_brightness | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.door_led_brightness | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.door_led_brightness | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.door_led_brightness | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.door_led_brightness | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.door_led_brightness | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.door_led_brightness | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.door_led_brightness | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.flags.Val | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.flags.Val | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.flags.Val | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.flags.Val | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.flags.Val | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.flags.Val | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.flags.announcement_enable | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.flags.announcement_enable | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.announcement_enable | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.announcement_enable | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.announcement_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.announcement_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.audio_path_test_enable | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.flags.audio_path_test_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.audio_path_test_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.audio_path_test_enable | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.audio_playback_enable | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.flags.audio_playback_enable | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.audio_playback_enable | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.audio_playback_enable | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.audio_playback_enable | src/src/test_handler.cpp | test_task | test harness |  | Y |  | TEST_ONLY;checked |
| configReg | configReg.flags.audio_playback_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.audio_playback_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.beacon_power_enabled | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.flags.beacon_power_enabled | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.beacon_power_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.beacon_power_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.beacon_power_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.charger_temperature_enable | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.charger_temperature_enable | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.charger_temperature_enable | src/src/power_handler.cpp | power_task | loop |  | Y |  | checked |
| configReg | configReg.flags.charger_temperature_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.charger_temperature_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.charger_temperature_enable | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.door_stuck_message_disable | src/main.cpp | log_task | loop |  | Y |  | checked |
| configReg | configReg.flags.door_stuck_message_disable | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.door_stuck_message_disable | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.door_stuck_message_disable | src/src/gpio_handler.cpp | gpio_task | loop |  | Y |  | checked |
| configReg | configReg.flags.door_stuck_message_disable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.door_stuck_message_disable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.door_stuck_message_disable | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.flags.door_stuck_message_disable | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.expansion_sim_enabled | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.flags.expansion_sim_enabled | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.expansion_sim_enabled | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.expansion_sim_enabled | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| configReg | configReg.flags.expansion_sim_enabled | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| configReg | configReg.flags.expansion_sim_enabled | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.flags.expansion_sim_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.expansion_sim_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.expansion_sim_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.flags.external_power_disable_enabled | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.flags.external_power_disable_enabled | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.external_power_disable_enabled | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.external_power_disable_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.external_power_disable_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.external_power_disable_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.ftp_secure_enabled | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.ftp_secure_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.ftp_secure_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.ftp_secure_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.gnss_enable | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.flags.gnss_enable | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.gnss_enable | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.gnss_enable | src/src/modem_handler.cpp | gps_task | task: modem |  | Y |  | checked |
| configReg | configReg.flags.gnss_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.gnss_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.gnss_enable | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.handset_loop_enable | src/main.cpp | log_task | loop |  | Y |  | checked |
| configReg | configReg.flags.handset_loop_enable | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.handset_loop_enable | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.handset_loop_enable | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.handset_loop_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.handset_loop_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.handset_loop_enable | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.imu_enabled | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.imu_enabled | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.imu_enabled | src/src/imu_handler.cpp | imu_task | loop |  | Y |  | checked |
| configReg | configReg.flags.imu_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.imu_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.imu_enabled | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.flags.imu_enabled | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.flags.imu_enabled | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| configReg | configReg.flags.imu_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.imu_vertical_swap_enabled | src/src/imu_handler.cpp | imu_task | loop |  | Y |  | checked |
| configReg | configReg.flags.imu_vertical_swap_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.imu_vertical_swap_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.imu_vertical_swap_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.io1_direction | src/main.cpp | log_task | loop |  | Y |  | checked |
| configReg | configReg.flags.io1_direction | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.flags.io1_direction | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.flags.io1_direction | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.io1_direction | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.io1_direction | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.flags.io1_trigger_enable | src/main.cpp | log_task | loop |  | Y |  | checked |
| configReg | configReg.flags.io1_trigger_enable | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.io1_trigger_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.io1_trigger_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.io1_trigger_enable | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.io2_direction | src/main.cpp | log_task | loop |  | Y |  | checked |
| configReg | configReg.flags.io2_direction | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.flags.io2_direction | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.flags.io2_direction | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.io2_direction | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.io2_direction | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.flags.io2_trigger_enable | src/main.cpp | log_task | loop |  | Y |  | checked |
| configReg | configReg.flags.io2_trigger_enable | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.io2_trigger_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.io2_trigger_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.io2_trigger_enable | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.led_board_status_enabled | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.led_board_status_enabled | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.led_board_status_enabled | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| configReg | configReg.flags.led_board_status_enabled | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.led_board_status_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.led_board_status_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.led_board_status_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.location_text_setup | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.location_text_setup | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.location_text_setup | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.flags.location_text_setup | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.location_text_setup | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.location_text_setup | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.location_text_setup | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.location_text_setup | src/src/wifi_handler.cpp | wifi_start_ap | loop |  | Y |  | checked |
| configReg | configReg.flags.low_signal_call_disable_enabled | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.flags.low_signal_call_disable_enabled | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.low_signal_call_disable_enabled | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.low_signal_call_disable_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.low_signal_call_disable_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.low_signal_call_disable_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.no_network_response_enabled | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.flags.no_network_response_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.no_network_response_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.flags.no_network_response_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.flags.ringer_audio_enable | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.flags.ringer_audio_enable | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.ringer_audio_enable | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.flags.ringer_audio_enable | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.flags.ringer_audio_enable | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.flags.ringer_audio_enable | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_directory | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_directory | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_directory | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.ftp_directory | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_directory | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_directory | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_directory | src/src/modem_handler.cpp | modem_ftp_directory | task: modem |  | Y |  | checked |
| configReg | configReg.ftp_directory | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.ftp_directory | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_directory | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_directory | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.ftp_last_update_date | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| configReg | configReg.ftp_last_update_date | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_last_update_date | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_last_update_date | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.ftp_last_update_date | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_last_update_date | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_last_update_date | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_last_update_date | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.ftp_last_update_date | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_password | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_password | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_password | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.ftp_password | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_password | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_password | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_password | src/src/modem_handler.cpp | modem_ftp_connect | task: modem |  | Y |  | checked |
| configReg | configReg.ftp_password | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.ftp_password | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.ftp_period | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.ftp_period | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_period | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_period | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.ftp_period | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_period | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_period | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_period | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.ftp_period | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.ftp_period | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_period | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_period | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.ftp_port | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_port | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_port | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.ftp_port | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_port | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_port | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_port | src/src/modem_handler.cpp | modem_ftp_connect | task: modem |  | Y |  | checked |
| configReg | configReg.ftp_port | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.ftp_port | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.ftp_port | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_port | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_port | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.ftp_server | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.ftp_server | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_server | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_server | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.ftp_server | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_server | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_server | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_server | src/src/modem_handler.cpp | modem_ftp_connect | task: modem |  | Y |  | checked |
| configReg | configReg.ftp_server | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.ftp_server | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.ftp_server | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_server | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_server | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.ftp_user | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.ftp_user | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_user | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ftp_user | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.ftp_user | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_user | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_user | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ftp_user | src/src/modem_handler.cpp | modem_ftp_connect | task: modem |  | Y |  | checked |
| configReg | configReg.ftp_user | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.ftp_user | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.ftp_user | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.handset_PGA_microphone_gain | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_microphone_gain | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_microphone_gain | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.handset_PGA_microphone_gain | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_microphone_gain | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_microphone_gain | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_microphone_gain | src/src/modem_handler.cpp | call_audio_setup | task: modem |  | Y |  | checked |
| configReg | configReg.handset_PGA_microphone_gain | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.handset_PGA_microphone_gain | src/src/test_handler.cpp | test_task | test harness |  | Y |  | TEST_ONLY;checked |
| configReg | configReg.handset_PGA_microphone_gain | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.handset_PGA_microphone_gain | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.handset_PGA_microphone_gain | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.handset_PGA_microphone_gain | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.handset_PGA_speaker_gain | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.handset_PGA_speaker_gain | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.handset_PGA_speaker_gain | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  | Y |  | checked |
| configReg | configReg.handset_PGA_speaker_gain | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_speaker_gain | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_speaker_gain | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.handset_PGA_speaker_gain | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_speaker_gain | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_speaker_gain | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_PGA_speaker_gain | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.handset_PGA_speaker_gain | src/src/test_handler.cpp | test_task | test harness |  | Y |  | TEST_ONLY;checked |
| configReg | configReg.handset_PGA_speaker_gain | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.handset_PGA_speaker_gain | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.handset_PGA_speaker_gain | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.handset_PGA_speaker_gain | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.handset_modem_microphone_gain | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_microphone_gain | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_microphone_gain | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.handset_modem_microphone_gain | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_microphone_gain | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_microphone_gain | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_microphone_gain | src/src/modem_handler.cpp | call_audio_setup | task: modem |  | Y |  | checked |
| configReg | configReg.handset_modem_microphone_gain | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.handset_modem_microphone_gain | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.handset_modem_microphone_gain | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.handset_modem_microphone_gain | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.handset_modem_microphone_gain | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.handset_modem_microphone_gain | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.handset_modem_speaker_volume | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_speaker_volume | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_speaker_volume | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.handset_modem_speaker_volume | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_speaker_volume | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_speaker_volume | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handset_modem_speaker_volume | src/src/modem_handler.cpp | call_audio_setup | task: modem |  | Y |  | checked |
| configReg | configReg.handset_modem_speaker_volume | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.handset_modem_speaker_volume | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.handset_modem_speaker_volume | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.handset_modem_speaker_volume | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.handset_modem_speaker_volume | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.handset_modem_speaker_volume | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.handset_modem_speaker_volume | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.handsfree_PGA_microphone_gain | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/modem_handler.cpp | call_audio_setup | task: modem |  | Y |  | checked |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.handsfree_PGA_microphone_gain | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.handsfree_PGA_speaker_gain | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  | Y |  | checked |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.handsfree_PGA_speaker_gain | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/modem_handler.cpp | call_audio_setup | task: modem |  | Y |  | checked |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.handsfree_modem_microphone_gain | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/modem_handler.cpp | call_audio_setup | task: modem |  | Y |  | checked |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.handsfree_modem_speaker_volume | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.imu_tip_threshold | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.imu_tip_threshold | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.imu_tip_threshold | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.imu_tip_threshold | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.imu_tip_threshold | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.imu_tip_threshold | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.imu_tip_threshold | src/src/imu_handler.cpp | imu_task | loop |  | Y |  | checked |
| configReg | configReg.imu_tip_threshold | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.imu_tip_threshold | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.imu_tip_threshold | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.imu_tip_threshold | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.imu_tip_threshold | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.load_on_period | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.load_on_period | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.load_on_period | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.load_on_period | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.load_on_period | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.load_on_period | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.load_on_period | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.load_on_period | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.load_on_period | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.load_on_period | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.load_on_period | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.load_on_period | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.log_text_period | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.log_text_period | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.log_text_period | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.log_text_period | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.log_text_period | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.log_text_period | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.log_text_period | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.log_text_period | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.log_text_period | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.log_text_period | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.log_text_period | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.log_text_period | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.log_text_period | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.log_text_period | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.logs.auth_log | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.logs.auth_log | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.logs.auth_log | src/src/modem_handler.cpp | log_auth_event | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.logs.auth_log | src/src/modem_handler.cpp | sms_prepare_unauth_parts | task: modem |  | Y |  | checked |
| configReg | configReg.logs.auth_log | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.logs.auth_log | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.logs.auth_log | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.logs.auth_log | src/src/wifi_handler.cpp | log_unauthorised_access | unknown | caller_not_in_slice | Y | Y | no callers in provided files |
| configReg | configReg.logs.sms_log | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.logs.sms_log | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.logs.sms_log | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.logs.sms_log | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.logs.sms_log | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.logs.sms_log | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.logs.sms_log | src/src/modem_handler.cpp | sms_build_smslog | task: modem |  | Y |  | checked |
| configReg | configReg.logs.sms_log | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.logs.sms_log | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.logs.sms_log | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.logs.sms_log | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.logs.web_log | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.logs.web_log | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.logs.web_log | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.logs.web_log | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.logs.web_log | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.logs.web_log | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.logs.web_log | src/src/modem_handler.cpp | sms_build_weblog | task: modem |  | Y |  | checked |
| configReg | configReg.logs.web_log | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.logs.web_log | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.logs.web_log | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.logs.web_log | src/src/wifi_handler.cpp | wifi_connect_event | wifi event callback |  | Y | Y | read-modify-write |
| configReg | configReg.mimic_dialtone_to_dial_time | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.mimic_dialtone_to_dial_time | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_intertone_period | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_intertone_period | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.mimic_dtmf_tone_period | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_tone_period | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_tone_period | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_tone_period | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.mimic_dtmf_tone_period | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_tone_period | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_tone_period | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_dtmf_tone_period | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.mimic_dtmf_tone_period | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_tone_period | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.mimic_dtmf_tone_period | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.mimic_dtmf_tone_period | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_tone_period | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_dtmf_tone_period | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.mimic_hook_to_dialtone | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.mimic_hook_to_dialtone | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.mimic_hook_to_dialtone | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_hook_to_dialtone | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_hook_to_dialtone | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.mimic_hook_to_dialtone | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_hook_to_dialtone | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_hook_to_dialtone | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_hook_to_dialtone | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.mimic_hook_to_dialtone | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.mimic_hook_to_dialtone | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.mimic_hook_to_dialtone | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.mimic_hook_to_dialtone | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_hook_to_dialtone | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_hook_to_dialtone | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.mimic_volume | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.mimic_volume | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.mimic_volume | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_volume | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.mimic_volume | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.mimic_volume | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_volume | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_volume | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.mimic_volume | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.mimic_volume | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.mimic_volume | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.mimic_volume | src/src/test_handler.cpp | test_task | test harness |  | Y |  | TEST_ONLY;checked |
| configReg | configReg.mimic_volume | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.mimic_volume | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_volume | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.mimic_volume | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.no_auth_logs | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.no_auth_logs | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.no_auth_logs | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.no_auth_logs | src/src/modem_handler.cpp | log_auth_event | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.no_auth_logs | src/src/modem_handler.cpp | sms_prepare_unauth_parts | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.no_auth_logs | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.no_auth_logs | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.no_auth_logs | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.no_auth_logs | src/src/wifi_handler.cpp | log_unauthorised_access | unknown | caller_not_in_slice | Y | Y | no callers in provided files |
| configReg | configReg.no_sms_logs | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.no_sms_logs | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.no_sms_logs | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.no_sms_logs | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.no_sms_logs | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.no_sms_logs | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.no_sms_logs | src/src/modem_handler.cpp | sms_build_smslog | task: modem |  | Y |  | checked |
| configReg | configReg.no_sms_logs | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.no_sms_logs | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.no_sms_logs | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.no_sms_logs | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.no_web_logs | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.no_web_logs | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.no_web_logs | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.no_web_logs | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.no_web_logs | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.no_web_logs | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.no_web_logs | src/src/modem_handler.cpp | sms_build_weblog | task: modem |  | Y |  | checked |
| configReg | configReg.no_web_logs | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.no_web_logs | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.no_web_logs | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.no_web_logs | src/src/wifi_handler.cpp | wifi_connect_event | wifi event callback |  | Y | Y | read-modify-write |
| configReg | configReg.phone_loc | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.phone_loc | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.phone_loc | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.phone_loc | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.phone_loc | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.phone_loc | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.phone_loc | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.phone_loc | src/src/modem_handler.cpp | sms_build_usertext | task: modem |  | Y |  | checked |
| configReg | configReg.phone_loc | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.phone_loc | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.phone_loc | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.phone_loc | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.phone_loc | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.phone_loc | src/src/wifi_handler.cpp | wifi_start_ap | loop |  | Y |  | checked |
| configReg | configReg.phone_serial | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.phone_serial | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.phone_serial | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.phone_serial | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.phone_serial | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.phone_serial | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.phone_serial | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.phone_serial | src/src/modem_handler.cpp | modem_ftp_directory | task: modem |  | Y |  | checked |
| configReg | configReg.phone_serial | src/src/modem_handler.cpp | sms_build_inftext | task: modem |  | Y |  | checked |
| configReg | configReg.phone_serial | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.phone_serial | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.phone_serial | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.phone_serial | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.phone_serial | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.ringer_volume | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.ringer_volume | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ringer_volume | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.ringer_volume | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.ringer_volume | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ringer_volume | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ringer_volume | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.ringer_volume | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.ringer_volume | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| configReg | configReg.ringer_volume | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| configReg | configReg.ringer_volume | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.ringer_volume | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.ringer_volume | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.ringer_volume | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.sms_logtext_no1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no1 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.sms_logtext_no1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no1 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.sms_logtext_no1 | src/src/modem_handler.cpp | sms_build_from_events | task: modem |  | Y |  | checked |
| configReg | configReg.sms_logtext_no1 | src/src/modem_handler.cpp | sms_build_inftext | task: modem |  | Y |  | checked |
| configReg | configReg.sms_logtext_no1 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.sms_logtext_no1 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.sms_logtext_no1 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.sms_logtext_no2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no2 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.sms_logtext_no2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no2 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.sms_logtext_no2 | src/src/modem_handler.cpp | sms_build_from_events | task: modem |  | Y |  | checked |
| configReg | configReg.sms_logtext_no2 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.sms_logtext_no2 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.sms_logtext_no2 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.sms_logtext_no3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no3 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.sms_logtext_no3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_logtext_no3 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.sms_logtext_no3 | src/src/modem_handler.cpp | sms_build_from_events | task: modem |  | Y |  | checked |
| configReg | configReg.sms_logtext_no3 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.sms_logtext_no3 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.sms_logtext_no3 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.sms_user_no1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no1 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.sms_user_no1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no1 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.sms_user_no1 | src/src/modem_handler.cpp | sms_build_from_events | task: modem |  | Y |  | checked |
| configReg | configReg.sms_user_no1 | src/src/modem_handler.cpp | sms_build_inftext | task: modem |  | Y |  | checked |
| configReg | configReg.sms_user_no1 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.sms_user_no1 | src/src/modem_handler.cpp | sms_queue_sim_notify | unknown | multi_context_function | Y |  | checked;multi caller contexts |
| configReg | configReg.sms_user_no1 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.sms_user_no1 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.sms_user_no1 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.sms_user_no1 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.sms_user_no2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no2 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.sms_user_no2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no2 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.sms_user_no2 | src/src/modem_handler.cpp | sms_build_from_events | task: modem |  | Y |  | checked |
| configReg | configReg.sms_user_no2 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.sms_user_no2 | src/src/modem_handler.cpp | sms_queue_sim_notify | unknown | multi_context_function | Y |  | checked;multi caller contexts |
| configReg | configReg.sms_user_no2 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.sms_user_no2 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.sms_user_no2 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.sms_user_no2 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.sms_user_no3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no3 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.sms_user_no3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.sms_user_no3 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.sms_user_no3 | src/src/modem_handler.cpp | sms_build_from_events | task: modem |  | Y |  | checked |
| configReg | configReg.sms_user_no3 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.sms_user_no3 | src/src/modem_handler.cpp | sms_queue_sim_notify | unknown | multi_context_function | Y |  | checked;multi caller contexts |
| configReg | configReg.sms_user_no3 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.sms_user_no3 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.sms_user_no3 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.sms_user_no3 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.status_led_brightness | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.status_led_brightness | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.status_led_brightness | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.status_led_brightness | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.status_led_brightness | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.status_led_brightness | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.status_led_brightness | src/src/leddisplay_handler.cpp | leddisplay_init | loop |  | Y |  | checked |
| configReg | configReg.status_led_brightness | src/src/leddisplay_handler.cpp | leddisplay_update_all | loop |  | Y | Y | read-modify-write |
| configReg | configReg.status_led_brightness | src/src/leddisplay_handler.cpp | leddisplay_update_single | loop |  | Y | Y | read-modify-write |
| configReg | configReg.status_led_brightness | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.status_led_brightness | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.status_led_brightness | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.status_led_brightness | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.status_led_brightness | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.telephone_operating_mode | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| configReg | configReg.telephone_operating_mode | src/main.cpp | submode_task | loop |  | Y | Y | read-modify-write |
| configReg | configReg.telephone_operating_mode | src/src/audio_handler.cpp | audio_test_task | task: audio |  | Y |  | checked |
| configReg | configReg.telephone_operating_mode | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.telephone_operating_mode | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.telephone_operating_mode | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.telephone_operating_mode | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.telephone_operating_mode | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.telephone_operating_mode | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.telephone_operating_mode | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| configReg | configReg.telephone_operating_mode | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y | Y | read-modify-write |
| configReg | configReg.telephone_operating_mode | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.telephone_operating_mode | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.telephone_operating_mode | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.telephone_operating_mode | src/src/wifi_handler.cpp | handleConfigJSON | http route handler |  | Y |  | checked |
| configReg | configReg.telephone_operating_mode | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.telephone_operating_mode | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| configReg | configReg.telephone_operating_mode | src/src/wifi_handler.cpp | handle_embedded | http route handler |  | Y |  | checked |
| configReg | configReg.tft_display_brightness | src/main.cpp | loop | loop |  | Y |  | checked |
| configReg | configReg.tft_display_brightness | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.tft_display_brightness | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.tft_display_brightness | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.tft_display_brightness | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.tft_display_brightness | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.tft_display_brightness | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.tft_display_brightness | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| configReg | configReg.tft_display_brightness | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.tft_display_brightness | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.tft_display_brightness | src/src/wifi_handler.cpp | handleConfigXML | http route handler |  | Y |  | checked |
| configReg | configReg.tft_display_brightness | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no1 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no1 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no1 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no1 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no1 | src/src/modem_handler.cpp | sms_build_inftext | task: modem |  | Y |  | checked |
| configReg | configReg.voice_no1 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no1 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no1 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no1 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no1 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no10 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no10 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no10 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no10 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no10 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no10 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no10 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no10 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no10 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no10 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no10 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no10 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no10 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no2 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no2 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no2 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no2 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no2 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no2 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no2 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no2 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no2 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no3 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no3 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no3 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no3 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no3 | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no3 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no3 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no3 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no3 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no4 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no4 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no4 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no4 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no4 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no4 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no4 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no4 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no4 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no4 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no4 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no4 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no4 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no5 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no5 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no5 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no5 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no5 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no5 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no5 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no5 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no5 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no5 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no5 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no5 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no5 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no6 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no6 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no6 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no6 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no6 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no6 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no6 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no6 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no6 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no6 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no6 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no6 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no6 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no7 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no7 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no7 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no7 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no7 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no7 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no7 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no7 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no7 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no7 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no7 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no7 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no7 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no8 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no8 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no8 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no8 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no8 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no8 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no8 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no8 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no8 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no8 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no8 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no8 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no8 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.voice_no9 | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| configReg | configReg.voice_no9 | src/main.cpp | submode_task | loop |  | Y |  | checked |
| configReg | configReg.voice_no9 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no9 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.voice_no9 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.voice_no9 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no9 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no9 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.voice_no9 | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  | Y | Y | read-modify-write |
| configReg | configReg.voice_no9 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.voice_no9 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no9 | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| configReg | configReg.voice_no9 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_enabled | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_enabled | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_enabled | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.whitelist_enabled | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_enabled | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_enabled | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_enabled | src/src/modem_handler.cpp | sms_whitelist_allowed | task: modem |  | Y |  | checked |
| configReg | configReg.whitelist_enabled | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_enabled | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_enabled | src/src/wifi_handler.cpp | handlePermissionsXML | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_enabled | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_inherit1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit1 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.whitelist_inherit1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit1 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_inherit1 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_inherit1 | src/src/wifi_handler.cpp | handlePermissionsXML | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_inherit1 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_inherit2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit2 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.whitelist_inherit2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit2 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_inherit2 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_inherit2 | src/src/wifi_handler.cpp | handlePermissionsXML | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_inherit2 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_inherit3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit3 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.whitelist_inherit3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_inherit3 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_inherit3 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_inherit3 | src/src/wifi_handler.cpp | handlePermissionsXML | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_inherit3 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_no1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no1 | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.whitelist_no1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no1 | src/src/modem_handler.cpp | sms_whitelist_allowed | task: modem |  | Y |  | checked |
| configReg | configReg.whitelist_no1 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_no1 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_no1 | src/src/wifi_handler.cpp | handlePermissionsXML | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_no1 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_no2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no2 | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.whitelist_no2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no2 | src/src/modem_handler.cpp | sms_whitelist_allowed | task: modem |  | Y |  | checked |
| configReg | configReg.whitelist_no2 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_no2 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_no2 | src/src/wifi_handler.cpp | handlePermissionsXML | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_no2 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_no3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no3 | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.whitelist_no3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.whitelist_no3 | src/src/modem_handler.cpp | sms_whitelist_allowed | task: modem |  | Y |  | checked |
| configReg | configReg.whitelist_no3 | src/src/wifi_handler.cpp | apply_config_from_json | http route handler |  |  | Y | written |
| configReg | configReg.whitelist_no3 | src/src/wifi_handler.cpp | build_config_json | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_no3 | src/src/wifi_handler.cpp | handlePermissionsXML | http route handler |  | Y |  | checked |
| configReg | configReg.whitelist_no3 | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_enabled1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled1 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_enabled1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled1 | src/src/wifi_handler.cpp | set_slot_enabled | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_enabled2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled2 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_enabled2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled2 | src/src/wifi_handler.cpp | set_slot_enabled | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_enabled2 | src/src/wifi_handler.cpp | slot_enabled | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_enabled3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled3 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_enabled3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_enabled3 | src/src/wifi_handler.cpp | set_slot_enabled | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_enabled3 | src/src/wifi_handler.cpp | slot_enabled | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_password1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password1 | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_password1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password1 | src/src/file_handler.cpp | migrate_legacy_passwords | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_password1 | src/src/wifi_handler.cpp | get_slot_hash | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_password1 | src/src/wifi_handler.cpp | set_slot_hash | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_password2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password2 | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_password2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password2 | src/src/file_handler.cpp | migrate_legacy_passwords | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_password2 | src/src/wifi_handler.cpp | get_slot_hash | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_password2 | src/src/wifi_handler.cpp | set_slot_hash | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_password3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password3 | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_password3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_password3 | src/src/file_handler.cpp | migrate_legacy_passwords | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_password3 | src/src/wifi_handler.cpp | get_slot_hash | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_password3 | src/src/wifi_handler.cpp | set_slot_hash | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_role1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role1 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_role1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role1 | src/src/wifi_handler.cpp | set_slot_role | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_role2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role2 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_role2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role2 | src/src/wifi_handler.cpp | get_slot_role | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_role2 | src/src/wifi_handler.cpp | set_slot_role | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_role3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role3 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_role3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_role3 | src/src/wifi_handler.cpp | get_slot_role | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_role3 | src/src/wifi_handler.cpp | set_slot_role | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_salt1 | src/src/file_handler.cpp | config_default | http route handler |  | Y | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt1 | src/src/file_handler.cpp | config_default | loop |  | Y | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt1 | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_salt1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt1 | src/src/file_handler.cpp | migrate_legacy_passwords | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_salt1 | src/src/wifi_handler.cpp | get_slot_salt | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_salt1 | src/src/wifi_handler.cpp | set_slot_salt | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_salt2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt2 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_salt2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt2 | src/src/file_handler.cpp | migrate_legacy_passwords | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_salt2 | src/src/wifi_handler.cpp | get_slot_salt | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_salt2 | src/src/wifi_handler.cpp | set_slot_salt | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_salt3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt3 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_salt3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_salt3 | src/src/file_handler.cpp | migrate_legacy_passwords | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_salt3 | src/src/wifi_handler.cpp | get_slot_salt | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_salt3 | src/src/wifi_handler.cpp | set_slot_salt | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_user1 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user1 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user1 | src/src/file_handler.cpp | config_read | loop |  | Y | Y | read-modify-write |
| configReg | configReg.wifi_web_user1 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user1 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user1 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user1 | src/src/wifi_handler.cpp | get_slot_username | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_user1 | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_user1 | src/src/wifi_handler.cpp | set_slot_username | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_user2 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user2 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user2 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_user2 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user2 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user2 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user2 | src/src/wifi_handler.cpp | get_slot_username | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_user2 | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_user2 | src/src/wifi_handler.cpp | set_slot_username | http route handler |  |  | Y | written |
| configReg | configReg.wifi_web_user3 | src/src/file_handler.cpp | config_default | http route handler |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user3 | src/src/file_handler.cpp | config_default | loop |  |  | Y | contexts=loop,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user3 | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| configReg | configReg.wifi_web_user3 | src/src/file_handler.cpp | config_write | http route handler |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user3 | src/src/file_handler.cpp | config_write | loop |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user3 | src/src/file_handler.cpp | config_write | task: modem |  | Y |  | contexts=loop,task: modem,http route handler,source=contexts_split |
| configReg | configReg.wifi_web_user3 | src/src/wifi_handler.cpp | get_slot_username | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_user3 | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| configReg | configReg.wifi_web_user3 | src/src/wifi_handler.cpp | set_slot_username | http route handler |  |  | Y | written |
| generalFlags | generalFlags.audio_test_abort_request | src/main.cpp | audio_test_clear_pending_requests | loop |  |  | Y | written |
| generalFlags | generalFlags.audio_test_abort_request | src/main.cpp | loop | loop |  |  | Y | written |
| generalFlags | generalFlags.audio_test_abort_request | src/src/audio_handler.cpp | audio_test_task | task: audio |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.audio_test_cleanup_request | src/main.cpp | audio_test_clear_pending_requests | loop |  |  | Y | written |
| generalFlags | generalFlags.audio_test_cleanup_request | src/main.cpp | loop | loop |  |  | Y | written |
| generalFlags | generalFlags.audio_test_cleanup_request | src/main.cpp | set_audio_cleanup_request | loop |  |  | Y | written |
| generalFlags | generalFlags.audio_test_cleanup_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.audio_test_request | src/main.cpp | audio_test_clear_pending_requests | loop |  |  | Y | written |
| generalFlags | generalFlags.audio_test_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.audio_test_request | src/src/audio_handler.cpp | audio_test_task | task: audio |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.audio_test_tone_start_request | src/main.cpp | audio_test_clear_pending_requests | loop |  |  | Y | written |
| generalFlags | generalFlags.audio_test_tone_start_request | src/main.cpp | loop | loop |  |  | Y | written |
| generalFlags | generalFlags.audio_test_tone_start_request | src/src/audio_handler.cpp | audio_test_task | task: audio |  |  | Y | written |
| generalFlags | generalFlags.audio_test_tone_start_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.call_answer_request | src/main.cpp | submode_task | loop |  |  | Y | written |
| generalFlags | generalFlags.call_answer_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.call_answer_request | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| generalFlags | generalFlags.call_audio_request | src/main.cpp | set_call_audio_request | loop |  |  | Y | written |
| generalFlags | generalFlags.call_audio_request | src/main.cpp | submode_task | loop |  |  | Y | written |
| generalFlags | generalFlags.call_audio_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.call_dtmf_request | src/main.cpp | submode_task | loop |  |  | Y | written |
| generalFlags | generalFlags.call_dtmf_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.call_hangup_request | src/main.cpp | submode_task | loop |  |  | Y | written |
| generalFlags | generalFlags.call_hangup_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.call_hangup_request | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| generalFlags | generalFlags.call_initialisation_request | src/main.cpp | submode_task | loop |  |  | Y | written |
| generalFlags | generalFlags.call_initialisation_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.call_initialisation_request | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| generalFlags | generalFlags.config_changed | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.config_changed | src/src/file_handler.cpp | config_read | loop |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | ftp_json_decode | Thingstream callback |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | log_auth_event | task: modem |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | sms_update_config | task: modem |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | sms_update_param | task: modem |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/modem_handler.cpp | sms_update_volume | task: modem |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/wifi_handler.cpp | handleConfigUpload | http route handler |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/wifi_handler.cpp | log_unauthorised_access | unknown | caller_not_in_slice |  | Y | no callers in provided files |
| generalFlags | generalFlags.config_changed | src/src/wifi_handler.cpp | password_matches | http route handler |  |  | Y | written |
| generalFlags | generalFlags.config_changed | src/src/wifi_handler.cpp | wifi_connect_event | wifi event callback |  |  | Y | written |
| generalFlags | generalFlags.external_voltage_request | src/src/adc_handler.cpp | adc_task | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.external_voltage_request | src/src/power_handler.cpp | power_task | loop |  |  | Y | written |
| generalFlags | generalFlags.force_led_update | src/main.cpp | loop | loop |  |  | Y | written |
| generalFlags | generalFlags.force_led_update | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.force_led_update | src/src/wifi_handler.cpp | handleDoUpdate | http route handler |  |  | Y | written |
| generalFlags | generalFlags.force_reset_request | src/main.cpp | loop | loop |  | Y |  | checked |
| generalFlags | generalFlags.force_reset_request | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| generalFlags | generalFlags.ftp_check_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_check_request | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| generalFlags | generalFlags.ftp_connect_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_connect_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_directory_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_directory_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_disconnect_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_disconnect_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_download_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_download_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_file_request | src/main.cpp | loop | loop |  | Y |  | checked |
| generalFlags | generalFlags.ftp_file_request | src/src/modem_handler.cpp | modem_binary_callback | unknown | indirect_dispatch | Y | Y | callback registration not visible |
| generalFlags | generalFlags.ftp_file_request | src/src/modem_handler.cpp | modem_callback | Thingstream callback |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_file_request | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| generalFlags | generalFlags.ftp_update_download_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_update_download_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_update_info_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_update_info_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_update_size_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.ftp_update_size_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.gnss_request | src/main.cpp | loop | loop |  |  | Y | written |
| generalFlags | generalFlags.gnss_request | src/src/modem_handler.cpp | gps_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.gpio_update_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.gpio_update_request | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| generalFlags | generalFlags.gpio_update_request | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| generalFlags | generalFlags.gpio_update_request | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  |  | Y | written |
| generalFlags | generalFlags.gpio_update_request | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| generalFlags | generalFlags.gpio_update_request | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| generalFlags | generalFlags.incoming_ring_request | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| generalFlags | generalFlags.incoming_ring_request | src/src/test_handler.cpp | test_task | test harness |  | Y | Y | TEST_ONLY;read-modify-write |
| generalFlags | generalFlags.load_activate_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.load_activate_request | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  |  | Y | written |
| generalFlags | generalFlags.modem_power_cycle_request | src/main.cpp | setup | setup |  |  | Y | written |
| generalFlags | generalFlags.modem_power_cycle_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.modem_reboot_request | src/src/button_handler.cpp | button_task | loop |  |  | Y | written |
| generalFlags | generalFlags.modem_reboot_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.modem_reboot_request | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| generalFlags | generalFlags.modem_reboot_request | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| generalFlags | generalFlags.modem_reboot_request | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  |  | Y | written |
| generalFlags | generalFlags.modem_reboot_request | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| generalFlags | generalFlags.power_status_request | src/main.cpp | loop | loop |  |  | Y | written |
| generalFlags | generalFlags.power_status_request | src/src/power_handler.cpp | power_task | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.sim_switch_reboot_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.sim_switch_reboot_request | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| generalFlags | generalFlags.sim_switch_reboot_request | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| generalFlags | generalFlags.sim_switch_reboot_request | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  |  | Y | written |
| generalFlags | generalFlags.sim_switch_reboot_request | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| generalFlags | generalFlags.sms_request_log | src/main.cpp | log_task | loop |  |  | Y | written |
| generalFlags | generalFlags.sms_request_log | src/main.cpp | loop | loop |  |  | Y | written |
| generalFlags | generalFlags.sms_request_log | src/src/modem_handler.cpp | modem_sms_busy | unknown | multi_context_function | Y |  | checked;multi caller contexts |
| generalFlags | generalFlags.sms_request_log | src/src/modem_handler.cpp | sms_abort_send | unknown | caller_not_in_slice |  | Y | no callers in provided files |
| generalFlags | generalFlags.sms_request_log | src/src/modem_handler.cpp | sms_collect_events | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.sms_request_user | src/src/modem_handler.cpp | modem_sms_busy | unknown | multi_context_function | Y |  | checked;multi caller contexts |
| generalFlags | generalFlags.sms_request_user | src/src/modem_handler.cpp | sms_abort_send | unknown | caller_not_in_slice |  | Y | no callers in provided files |
| generalFlags | generalFlags.sms_request_user | src/src/modem_handler.cpp | sms_collect_events | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.sms_request_user | src/src/modem_handler.cpp | sms_queue_sim_notify | unknown | multi_context_function |  | Y | written;multi caller contexts |
| generalFlags | generalFlags.sms_request_user | src/src/modem_handler.cpp | sms_send_user | task: modem |  |  | Y | written |
| generalFlags | generalFlags.sms_request_user | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| generalFlags | generalFlags.sms_request_user | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| generalFlags | generalFlags.test_audio_request | src/main.cpp | audio_test_clear_pending_requests | loop |  |  | Y | written |
| generalFlags | generalFlags.test_audio_request | src/main.cpp | loop | loop |  |  | Y | written |
| generalFlags | generalFlags.test_audio_request | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.wifi_deactivate_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.wifi_deactivate_request | src/main.cpp | submode_task | loop |  |  | Y | written |
| generalFlags | generalFlags.wifi_deactivate_request | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| generalFlags | generalFlags.wifi_request | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| generalFlags | generalFlags.wifi_request | src/src/button_handler.cpp | button_task | loop |  |  | Y | written |
| generalFlags | generalFlags.wifi_request | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| gpio_controls | gpio_controls.beacon_control | src/main.cpp | loop | loop |  |  | Y | written |
| gpio_controls | gpio_controls.beacon_control | src/main.cpp | submode_task | loop |  |  | Y | written |
| gpio_controls | gpio_controls.beacon_control | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| gpio_controls | gpio_controls.charge_enable | src/main.cpp | loop | loop |  |  | Y | written |
| gpio_controls | gpio_controls.charge_enable | src/src/power_handler.cpp | power_set_charge_status | unknown | caller_not_in_slice |  | Y | no callers in provided files |
| gpio_controls | gpio_controls.charge_enable | src/src/power_handler.cpp | power_task | loop |  |  | Y | written |
| gpio_controls | gpio_controls.handsfree_control | src/main.cpp | loop | loop |  |  | Y | written |
| gpio_controls | gpio_controls.handsfree_control | src/main.cpp | submode_task | loop |  |  | Y | written |
| gpio_controls | gpio_controls.handsfree_control | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| gpio_controls | gpio_controls.hshf_select | src/main.cpp | loop | loop |  |  | Y | written |
| gpio_controls | gpio_controls.hshf_select | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| gpio_controls | gpio_controls.io1_control | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| gpio_controls | gpio_controls.io1_control | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| gpio_controls | gpio_controls.io1_control | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| gpio_controls | gpio_controls.io2_control | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| gpio_controls | gpio_controls.io2_control | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| gpio_controls | gpio_controls.io2_control | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| gpio_controls | gpio_controls.load_control | src/main.cpp | loop | loop |  |  | Y | written |
| gpio_controls | gpio_controls.load_control | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  |  | Y | written |
| gpio_controls | gpio_controls.load_control | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| gpio_controls | gpio_controls.ringer_control | src/main.cpp | loop | loop |  |  | Y | written |
| gpio_controls | gpio_controls.ringer_control | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| gpio_controls | gpio_controls.sim_select_control | src/main.cpp | loop | loop |  |  | Y | written |
| gpio_controls | gpio_controls.sim_select_control | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| gpio_controls | gpio_controls.sim_select_control | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| gpio_controls | gpio_controls.sim_select_control | src/src/modem_handler.cpp | sim_switch_guard_start | unknown | multi_context_function | Y |  | checked;multi caller contexts |
| gpio_controls | gpio_controls.sim_select_control | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  |  | Y | written |
| gpio_controls | gpio_controls.sim_select_control | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| statusReg | statusReg.audio_test_tone_tick | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.audio_test_tone_tick | src/src/audio_handler.cpp | audio_test_task | task: audio |  | Y | Y | read-modify-write |
| statusReg | statusReg.audio_test_tone_tick | src/src/modem_handler.cpp | audio_test_cleanup | task: modem |  |  | Y | written |
| statusReg | statusReg.audio_test_tone_tick | src/src/modem_handler.cpp | audio_test_tone_start | task: modem |  |  | Y | written |
| statusReg | statusReg.audio_test_tone_tick | src/src/modem_handler.cpp | test_audio_setup | task: modem |  |  | Y | written |
| statusReg | statusReg.battery_voltage | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.battery_voltage | src/src/modem_handler.cpp | sms_build_usertext | task: modem |  | Y |  | checked |
| statusReg | statusReg.battery_voltage | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.call_status | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.call_status | src/main.cpp | submode_task | loop |  |  | Y | written |
| statusReg | statusReg.call_status | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.current_timedate | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.current_timedate | src/src/modem_handler.cpp | log_auth_event | task: modem |  | Y |  | checked |
| statusReg | statusReg.current_timedate | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y |  | checked |
| statusReg | statusReg.current_timedate | src/src/wifi_handler.cpp | log_unauthorised_access | unknown | caller_not_in_slice | Y |  | no callers in provided files |
| statusReg | statusReg.current_timedate | src/src/wifi_handler.cpp | wifi_connect_event | wifi event callback |  | Y |  | checked |
| statusReg | statusReg.external_voltage | src/src/adc_handler.cpp | adc_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.external_voltage | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.external_voltage | src/src/modem_handler.cpp | sms_build_usertext | task: modem |  | Y |  | checked |
| statusReg | statusReg.external_voltage | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.flags.audio_digit_playing | src/main.cpp | submode_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.audio_digit_playing | src/src/audio_handler.cpp | dtmf_task | task: audio |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.audio_test_cleanup_complete | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.audio_test_cleanup_complete | src/src/modem_handler.cpp | audio_test_cleanup | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.audio_test_complete | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.audio_test_complete | src/src/audio_handler.cpp | audio_test_task | task: audio |  |  | Y | written |
| statusReg | statusReg.flags.audio_test_passed | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.audio_test_passed | src/src/audio_handler.cpp | audio_test_task | task: audio |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.audio_test_tone_active | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.audio_test_tone_active | src/src/audio_handler.cpp | audio_test_task | task: audio |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.audio_test_tone_active | src/src/modem_handler.cpp | audio_test_cleanup | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.audio_test_tone_active | src/src/modem_handler.cpp | audio_test_tone_start | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.audio_test_tone_active | src/src/modem_handler.cpp | test_audio_setup | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.beacon_status | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.beacon_status | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.flags.call_connected | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.call_connected | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.call_connected | src/main.cpp | submode_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.call_connected | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.call_disconnected | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.call_disconnected | src/src/test_handler.cpp | test_task | test harness |  | Y | Y | TEST_ONLY;read-modify-write |
| statusReg | statusReg.flags.call_inprogress | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.call_inprogress | src/src/test_handler.cpp | test_task | test harness |  | Y | Y | TEST_ONLY;read-modify-write |
| statusReg | statusReg.flags.call_volte_active | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.call_volte_active | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.call_volte_active | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.door_open | src/main.cpp | door_illumination_process | loop |  | Y |  | checked |
| statusReg | statusReg.flags.door_open | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.door_open | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.door_open | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.flags.door_open | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y |  | checked |
| statusReg | statusReg.flags.door_stuck | src/main.cpp | log_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.door_stuck | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.doorstuck_logtext_done | src/main.cpp | log_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.doorstuck_logtext_done | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.doorstuck_logtext_done | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.ftp_directory_not_found | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.ftp_directory_not_found | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.ftp_download_complete | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.ftp_download_complete | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.ftp_file_not_found | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.ftp_file_not_found | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.ftp_open_failed | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.ftp_open_failed | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.gnss_fixed | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.gnss_fixed | src/src/modem_handler.cpp | gps_task | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.handset_loop_open | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handset_loop_open | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.handset_loop_open | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button1_pressed | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handsfree_button1_pressed | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button1_pressed | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button1_pressed | src/src/test_handler.cpp | test_task | test harness |  | Y | Y | TEST_ONLY;read-modify-write |
| statusReg | statusReg.flags.handsfree_button1_released | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button1_released | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button1_released | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button1_stuck | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handsfree_button1_stuck | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button2_pressed | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handsfree_button2_pressed | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button2_pressed | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button2_pressed | src/src/test_handler.cpp | test_task | test harness |  | Y | Y | TEST_ONLY;read-modify-write |
| statusReg | statusReg.flags.handsfree_button2_released | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button2_released | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button2_released | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button2_stuck | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handsfree_button2_stuck | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button3_pressed | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handsfree_button3_pressed | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button3_pressed | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button3_pressed | src/src/test_handler.cpp | test_task | test harness |  | Y | Y | TEST_ONLY;read-modify-write |
| statusReg | statusReg.flags.handsfree_button3_released | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button3_released | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button3_stuck | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handsfree_button3_stuck | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button4_pressed | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handsfree_button4_pressed | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.handsfree_button4_pressed | src/src/test_handler.cpp | test_task | test harness |  | Y | Y | TEST_ONLY;read-modify-write |
| statusReg | statusReg.flags.handsfree_button4_released | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button4_released | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button4_released | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.handsfree_button4_stuck | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.handsfree_button4_stuck | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.hook_button_pressed | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.hook_button_pressed | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.hook_button_pressed | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.hook_button_released | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.hook_button_released | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.hook_button_released | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.hook_button_stuck | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.hook_button_stuck | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.imu_tilt_exceeded | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.imu_tilt_exceeded | src/src/imu_handler.cpp | imu_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.imu_tilt_logtext_done | src/main.cpp | log_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.imu_tilt_logtext_done | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.imu_tilt_logtext_done | src/src/imu_handler.cpp | imu_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.imu_tilt_reset | src/main.cpp | log_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.imu_tilt_reset | src/src/imu_handler.cpp | imu_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.imu_tilt_reset_logtext_done | src/main.cpp | log_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.imu_tilt_reset_logtext_done | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.imu_tilt_reset_logtext_done | src/src/imu_handler.cpp | imu_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.incoming_call_ring_detected | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.incoming_call_ring_detected | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.incoming_call_ringing | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.incoming_call_ringing | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.incoming_call_ringing | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| statusReg | statusReg.flags.incoming_sms_ready | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.incoming_sms_ready | src/src/modem_handler.cpp | modem_callback | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.incoming_sms_ready | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.io1_input_status | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.io1_input_status | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.flags.io1_logtext_done | src/main.cpp | log_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.io1_logtext_done | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.io1_output_status | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.io1_output_status | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.flags.io1_output_status | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| statusReg | statusReg.flags.io1_trigger_status | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.io1_trigger_status | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.io2_input_status | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.io2_input_status | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.flags.io2_logtext_done | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.io2_logtext_done | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.io2_output_status | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.io2_output_status | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.flags.io2_output_status | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| statusReg | statusReg.flags.io2_trigger_status | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.io2_trigger_status | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_dtr_status | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_dtr_status | src/src/modem_handler.cpp | modem_sleep | unknown | multi_context_function |  | Y | written;multiple caller contexts |
| statusReg | statusReg.flags.modem_dtr_status | src/src/modem_handler.cpp | modem_wake | unknown | multi_context_function |  | Y | written;multiple caller contexts |
| statusReg | statusReg.flags.modem_first_registration | src/src/button_handler.cpp | button_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_first_registration | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_first_registration | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.modem_first_registration | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| statusReg | statusReg.flags.modem_first_registration | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  |  | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| statusReg | statusReg.flags.modem_first_registration | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_first_registration | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| statusReg | statusReg.flags.modem_ftp_connected | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.modem_ftp_connected | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.modem_no_sim | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_no_sim | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.modem_no_sim | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.modem_no_sim | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  | Y | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| statusReg | statusReg.flags.modem_no_sim | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  | Y | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| statusReg | statusReg.flags.modem_no_sim | src/src/modem_handler.cpp | sim_switch_guard_start | unknown | multi_context_function |  | Y | written;multiple caller contexts |
| statusReg | statusReg.flags.modem_no_sim | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y |  | checked |
| statusReg | statusReg.flags.modem_process_complete | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.modem_process_complete | src/main.cpp | submode_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | call_answer | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | call_audio_setup | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | call_initialisation | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | modem_ftp_connect | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | modem_ftp_directory | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | modem_ftp_disconnect | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | modem_ftp_download | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | modem_ftp_update_download | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | modem_ftp_update_fileinfo | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | modem_ftp_update_filesize | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_complete | src/src/modem_handler.cpp | test_audio_setup | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_inprogress | src/main.cpp | door_illumination_process | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_process_inprogress | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.modem_process_inprogress | src/src/modem_handler.cpp | gps_task | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_process_inprogress | src/src/modem_handler.cpp | modem_task | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_registered | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_registered | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_registered | src/src/button_handler.cpp | button_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_registered | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.modem_registered | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.modem_registered | src/src/modem_handler.cpp | sim_switch_guard_handle | Thingstream callback |  | Y | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| statusReg | statusReg.flags.modem_registered | src/src/modem_handler.cpp | sim_switch_guard_handle | task: modem |  | Y | Y | contexts=task: modem,Thingstream callback,source=contexts_split |
| statusReg | statusReg.flags.modem_registered | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.modem_registered | src/src/test_handler.cpp | test_task | test harness |  | Y |  | TEST_ONLY;checked |
| statusReg | statusReg.flags.modem_registered | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.flags.modem_registered | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.off_hook_active | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.off_hook_active | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.off_hook_active | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| statusReg | statusReg.flags.sms_command_processed | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.sms_command_processed | src/src/modem_handler.cpp | sms_decode | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.sms_inprogress | src/main.cpp | door_illumination_process | loop |  | Y |  | checked |
| statusReg | statusReg.flags.sms_inprogress | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.sms_inprogress | src/src/modem_handler.cpp | modem_sms_busy | unknown | multi_context_function | Y |  | checked;multiple caller contexts |
| statusReg | statusReg.flags.sms_inprogress | src/src/modem_handler.cpp | sms_abort_send | unknown | caller_not_in_slice |  | Y | no callers in provided files |
| statusReg | statusReg.flags.sms_inprogress | src/src/modem_handler.cpp | sms_reset_send_state | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.sms_inprogress | src/src/modem_handler.cpp | sms_task | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.system_booting | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.flags.system_booting | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.system_booting | src/src/modem_handler.cpp | modem_task | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.tft_status | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.tft_status | src/src/tft_handler.cpp | tft_deinit | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.tft_status | src/src/tft_handler.cpp | tft_init | loop |  |  | Y | written |
| statusReg | statusReg.flags.timedate_set | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.timedate_set | src/src/modem_handler.cpp | gps_set_datatime | task: modem |  |  | Y | written |
| statusReg | statusReg.flags.timedate_set | src/src/modem_handler.cpp | modem_set_datatime | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.flags.volume_button_dwn_pressed | src/main.cpp | submode_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.volume_button_dwn_pressed | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.volume_button_dwn_pressed | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.volume_button_dwn_released | src/main.cpp | submode_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.volume_button_dwn_released | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.volume_button_dwn_released | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.volume_button_dwn_stuck | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.volume_button_dwn_stuck | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.volume_button_up_pressed | src/main.cpp | submode_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.volume_button_up_pressed | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.volume_button_up_pressed | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.volume_button_up_released | src/main.cpp | submode_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.volume_button_up_released | src/src/gpio_handler.cpp | gpio_initialise | loop |  |  | Y | written |
| statusReg | statusReg.flags.volume_button_up_released | src/src/gpio_handler.cpp | gpio_task | loop |  |  | Y | written |
| statusReg | statusReg.flags.volume_button_up_stuck | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.volume_button_up_stuck | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.wifi_server_connected | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.wifi_server_connected | src/src/wifi_handler.cpp | wifi_check_client_timeouts | unknown | multi_context_function | Y | Y | read-modify-write;multiple caller contexts |
| statusReg | statusReg.flags.wifi_server_connected | src/src/wifi_handler.cpp | wifi_connect_event | wifi event callback |  |  | Y | written |
| statusReg | statusReg.flags.wifi_server_connected | src/src/wifi_handler.cpp | wifi_diagnostic_info | unknown | caller_not_in_slice | Y |  | checked;no callers in slice |
| statusReg | statusReg.flags.wifi_server_connected | src/src/wifi_handler.cpp | wifi_disconnect_event | wifi event callback |  |  | Y | written |
| statusReg | statusReg.flags.wifi_server_connected | src/src/wifi_handler.cpp | wifi_stop_ap | loop |  |  | Y | written |
| statusReg | statusReg.flags.wifi_server_disconnected | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.wifi_server_disconnected | src/src/wifi_handler.cpp | wifi_check_client_timeouts | unknown | multi_context_function |  | Y | written;multiple caller contexts |
| statusReg | statusReg.flags.wifi_server_disconnected | src/src/wifi_handler.cpp | wifi_connect_event | wifi event callback |  |  | Y | written |
| statusReg | statusReg.flags.wifi_server_disconnected | src/src/wifi_handler.cpp | wifi_diagnostic_info | unknown | caller_not_in_slice | Y |  | checked;no callers in slice |
| statusReg | statusReg.flags.wifi_server_disconnected | src/src/wifi_handler.cpp | wifi_disconnect_event | wifi event callback |  |  | Y | written |
| statusReg | statusReg.flags.wifi_server_disconnected | src/src/wifi_handler.cpp | wifi_stop_ap | loop |  |  | Y | written |
| statusReg | statusReg.flags.wifi_server_started | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.flags.wifi_server_started | src/main.cpp | submode_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.wifi_server_started | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.wifi_server_started | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y |  | checked |
| statusReg | statusReg.flags.wifi_server_started | src/src/wifi_handler.cpp | wifi_diagnostic_info | unknown | caller_not_in_slice | Y |  | checked;no callers in slice |
| statusReg | statusReg.flags.wifi_server_started | src/src/wifi_handler.cpp | wifi_start_ap | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.flags.wifi_server_started | src/src/wifi_handler.cpp | wifi_stop_ap | loop |  |  | Y | written |
| statusReg | statusReg.flags.wifi_update_in_progress | src/src/leddisplay_handler.cpp | leddisplay_task | loop |  | Y |  | checked |
| statusReg | statusReg.flags.wifi_update_in_progress | src/src/wifi_handler.cpp | handleDoUpdate | http route handler |  |  | Y | written |
| statusReg | statusReg.ftp_file_date | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.ftp_file_date | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.ftp_file_size | src/main.cpp | loop | loop |  | Y |  | checked |
| statusReg | statusReg.ftp_file_size | src/src/modem_handler.cpp | modem_binary_callback | unknown | indirect_dispatch | Y |  | callback registration not visible |
| statusReg | statusReg.ftp_file_size | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.imu_pitch | src/src/imu_handler.cpp | imu_task | loop |  |  | Y | written |
| statusReg | statusReg.imu_pitch | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.imu_pitch | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.imu_roll | src/src/imu_handler.cpp | imu_task | loop |  |  | Y | written |
| statusReg | statusReg.imu_roll | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.imu_roll | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.keypad_keypress | src/main.cpp | submode_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.keypad_keypress | src/src/gpio_handler.cpp | gpio_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.keypad_keypress | src/src/test_handler.cpp | test_task | test harness |  | Y | Y | TEST_ONLY;read-modify-write |
| statusReg | statusReg.last_call_length | src/main.cpp | log_task | loop |  | Y |  | checked |
| statusReg | statusReg.last_call_length | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.last_call_length | src/main.cpp | submode_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.last_call_length | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.last_call_length | src/src/modem_handler.cpp | sms_build_usertext | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_call_status | src/main.cpp | submode_task | loop |  | Y |  | checked |
| statusReg | statusReg.modem_call_status | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  | Y |  | checked |
| statusReg | statusReg.modem_call_status_counter | src/main.cpp | submode_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_call_status_counter | src/src/modem_handler.cpp | modem_call_status_reset | loop |  |  | Y | written |
| statusReg | statusReg.modem_call_status_counter | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_cell_ID | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.modem_cell_ID | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_cell_ID | src/src/modem_handler.cpp | sms_build_inftext | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_cell_operator | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.modem_cell_operator | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_cell_operator | src/src/modem_handler.cpp | sms_build_inftext | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_dialling_number | src/main.cpp | mimic_dial | loop |  | Y |  | checked |
| statusReg | statusReg.modem_dialling_number | src/main.cpp | submode_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_dialling_number | src/src/modem_handler.cpp | call_initialisation | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_dtmf_digit | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.modem_dtmf_digit | src/main.cpp | submode_task | loop |  |  | Y | written |
| statusReg | statusReg.modem_dtmf_digit | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  | Y |  | checked |
| statusReg | statusReg.modem_dtmf_digit | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.modem_dtmf_digit | src/src/modem_handler.cpp | modem_task | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_originator_number | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.modem_originator_number | src/src/modem_handler.cpp | modem_task | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_originator_number | src/src/modem_handler.cpp | sms_abort_send | unknown | caller_not_in_slice |  | Y | no callers in provided files |
| statusReg | statusReg.modem_originator_number | src/src/modem_handler.cpp | sms_apply_esim | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_originator_number | src/src/modem_handler.cpp | sms_collect_events | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_originator_number | src/src/modem_handler.cpp | sms_decode | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_originator_number | src/src/modem_handler.cpp | sms_queue_sim_notify | unknown | multi_context_function | Y | Y | read-modify-write;multiple caller contexts |
| statusReg | statusReg.modem_originator_number | src/src/modem_handler.cpp | sms_whitelist_allowed | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_originator_number | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| statusReg | statusReg.modem_originator_number | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_response | src/main.cpp | door_illumination_process | loop |  |  | Y | written |
| statusReg | statusReg.modem_response | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.modem_response | src/main.cpp | submode_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_response | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  |  | Y | written |
| statusReg | statusReg.modem_response | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.modem_response | src/src/modem_handler.cpp | modem_send | task: modem |  |  | Y | written |
| statusReg | statusReg.modem_response | src/src/modem_handler.cpp | modem_task | task: modem |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_response | src/src/modem_handler.cpp | sms_task | task: modem |  | Y | Y | read-modify-write |
| statusReg | statusReg.modem_rx_level | src/main.cpp | submode_task | loop |  | Y |  | checked |
| statusReg | statusReg.modem_rx_level | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.modem_rx_level | src/src/modem_handler.cpp | sms_build_usertext | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_signal_level | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  |  | Y | written |
| statusReg | statusReg.modem_signal_level | src/src/modem_handler.cpp | modem_task | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_signal_level | src/src/modem_handler.cpp | sms_build_logtext | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_signal_level | src/src/modem_handler.cpp | sms_build_usertext | task: modem |  | Y |  | checked |
| statusReg | statusReg.modem_signal_level | src/src/wifi_handler.cpp | handleValuesXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.sim_number | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.sim_number | src/src/modem_handler.cpp | modem_decode | Thingstream callback |  | Y | Y | read-modify-write |
| statusReg | statusReg.sim_number | src/src/wifi_handler.cpp | handleSetupXML | http route handler |  | Y |  | checked |
| statusReg | statusReg.sms_log_type | src/main.cpp | log_task | loop |  |  | Y | written |
| statusReg | statusReg.sms_log_type | src/main.cpp | loop | loop |  |  | Y | written |
| statusReg | statusReg.sms_log_type | src/src/modem_handler.cpp | sms_collect_events | task: modem |  | Y |  | checked |
| statusReg | statusReg.sms_log_type | src/src/modem_handler.cpp | sms_queue_sim_notify | unknown | multi_context_function |  | Y | written;multiple caller contexts |
| statusReg | statusReg.sms_log_type | src/src/modem_handler.cpp | sms_send_user | task: modem |  |  | Y | written |
| statusReg | statusReg.sms_log_type | src/src/wifi_handler.cpp | handle_command_requests | http route handler |  |  | Y | written |
| statusReg | statusReg.telephone_mode | src/main.cpp | door_illumination_process | loop |  |  | Y | written |
| statusReg | statusReg.telephone_mode | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.telephone_mode | src/main.cpp | submode_task | loop |  |  | Y | written |
| statusReg | statusReg.telephone_mode | src/src/dtmf_handler.cpp | dtmf_decode_task | loop |  |  | Y | written |
| statusReg | statusReg.telephone_mode | src/src/modem_handler.cpp | call_answer | task: modem |  |  | Y | written |
| statusReg | statusReg.telephone_mode | src/src/modem_handler.cpp | call_audio_setup | task: modem |  |  | Y | written |
| statusReg | statusReg.telephone_mode | src/src/modem_handler.cpp | call_initialisation | task: modem |  |  | Y | written |
| statusReg | statusReg.telephone_mode | src/src/test_handler.cpp | test_task | test harness |  |  | Y | TEST_ONLY;written |
| statusReg | statusReg.telephone_sub_mode | src/main.cpp | loop | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.telephone_sub_mode | src/main.cpp | submode_task | loop |  | Y | Y | read-modify-write |
| statusReg | statusReg.telephone_sub_mode | src/src/modem_handler.cpp | call_initialisation | task: modem |  |  | Y | written |
