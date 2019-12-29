################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
dispatcher.obj: C:/ti/CC3000\ SDK/MSP430FR5739/Basic\ WiFi\ Application/Basic\ WiFi\ Source/Source/HyperTerminal\ Driver/dispatcher.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O4 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/CC3000 SDK/MSP430FR5739/Basic WiFi Application/Basic WiFi Source/Source/CC3000HostDriver" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=__CCS__ --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="dispatcher.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


