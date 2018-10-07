################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\AbstractEvent.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\AnalogSensor.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\BeeperB.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\ButtonB.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\EspSettingsBox.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\KeyValue.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\KeyValueContainer.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\LedB.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\MqttHelper.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\Pir.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\TimeTrigger.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\WiFiHelper.cpp 

LINK_OBJ += \
.\libraries\TimeTrigger\AbstractEvent.cpp.o \
.\libraries\TimeTrigger\AnalogSensor.cpp.o \
.\libraries\TimeTrigger\BeeperB.cpp.o \
.\libraries\TimeTrigger\ButtonB.cpp.o \
.\libraries\TimeTrigger\EspSettingsBox.cpp.o \
.\libraries\TimeTrigger\KeyValue.cpp.o \
.\libraries\TimeTrigger\KeyValueContainer.cpp.o \
.\libraries\TimeTrigger\LedB.cpp.o \
.\libraries\TimeTrigger\MqttHelper.cpp.o \
.\libraries\TimeTrigger\Pir.cpp.o \
.\libraries\TimeTrigger\TimeTrigger.cpp.o \
.\libraries\TimeTrigger\WiFiHelper.cpp.o 

CPP_DEPS += \
.\libraries\TimeTrigger\AbstractEvent.cpp.d \
.\libraries\TimeTrigger\AnalogSensor.cpp.d \
.\libraries\TimeTrigger\BeeperB.cpp.d \
.\libraries\TimeTrigger\ButtonB.cpp.d \
.\libraries\TimeTrigger\EspSettingsBox.cpp.d \
.\libraries\TimeTrigger\KeyValue.cpp.d \
.\libraries\TimeTrigger\KeyValueContainer.cpp.d \
.\libraries\TimeTrigger\LedB.cpp.d \
.\libraries\TimeTrigger\MqttHelper.cpp.d \
.\libraries\TimeTrigger\Pir.cpp.d \
.\libraries\TimeTrigger\TimeTrigger.cpp.d \
.\libraries\TimeTrigger\WiFiHelper.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\TimeTrigger\AbstractEvent.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\AbstractEvent.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\AnalogSensor.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\AnalogSensor.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\BeeperB.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\BeeperB.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\ButtonB.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\ButtonB.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\EspSettingsBox.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\EspSettingsBox.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\KeyValue.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\KeyValue.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\KeyValueContainer.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\KeyValueContainer.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\LedB.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\LedB.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\MqttHelper.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\MqttHelper.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\Pir.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\Pir.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\TimeTrigger.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\TimeTrigger.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\WiFiHelper.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\WiFiHelper.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\workspace\MQTT_Switch_ON_OFF/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


