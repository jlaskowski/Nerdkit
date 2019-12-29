################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
cc3000_common.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/CC3000HostDriver/cc3000_common.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/inc" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=MDNS_ADVERTISE_HOST --define=__CCS__ --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --asm_listing --preproc_with_compile --preproc_dependency="cc3000_common.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

evnt_handler.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/CC3000HostDriver/evnt_handler.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/inc" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=MDNS_ADVERTISE_HOST --define=__CCS__ --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --asm_listing --preproc_with_compile --preproc_dependency="evnt_handler.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

hci.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/CC3000HostDriver/hci.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/inc" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=MDNS_ADVERTISE_HOST --define=__CCS__ --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --asm_listing --preproc_with_compile --preproc_dependency="hci.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

netapp.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/CC3000HostDriver/netapp.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/inc" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=MDNS_ADVERTISE_HOST --define=__CCS__ --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --asm_listing --preproc_with_compile --preproc_dependency="netapp.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

nvmem.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/CC3000HostDriver/nvmem.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/inc" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=MDNS_ADVERTISE_HOST --define=__CCS__ --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --asm_listing --preproc_with_compile --preproc_dependency="nvmem.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

security.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/CC3000HostDriver/security.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/inc" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=MDNS_ADVERTISE_HOST --define=__CCS__ --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --asm_listing --preproc_with_compile --preproc_dependency="security.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

socket.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/CC3000HostDriver/socket.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/inc" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=MDNS_ADVERTISE_HOST --define=__CCS__ --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --asm_listing --preproc_with_compile --preproc_dependency="socket.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

wlan.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/CC3000HostDriver/wlan.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/inc" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=MDNS_ADVERTISE_HOST --define=__CCS__ --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --asm_listing --preproc_with_compile --preproc_dependency="wlan.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


