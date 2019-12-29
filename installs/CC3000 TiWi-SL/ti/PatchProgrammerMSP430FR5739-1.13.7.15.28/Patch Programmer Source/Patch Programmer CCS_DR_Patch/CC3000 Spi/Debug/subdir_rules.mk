################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
spi.obj: D:/CC3000/Release/1.13/PatchProgrammer/PatchProgrammerMSP430FR5739-1.13.7.15.28/Patch\ Programmer\ Source/Source/CC3000\ Spi/spi.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.2.1/bin/cl430" -vmspx --abi=coffabi -O0 --opt_for_speed=0 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="D:/CC3000/Release/1.13/PatchProgrammer/PatchProgrammerMSP430FR5739-1.13.7.15.28/Patch Programmer Source/Source/inc" --include_path="D:/CC3000/Release/1.13/PatchProgrammer/PatchProgrammerMSP430FR5739-1.13.7.15.28/Patch Programmer Source/Source/CC3000HostDriver" --include_path="D:/CC3000/Release/1.13/PatchProgrammer/PatchProgrammerMSP430FR5739-1.13.7.15.28/Patch Programmer Source/Source/Patch Programmer" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.2.1/include" --gcc --define=__CCS__ --define=CC3000_TINY_DRIVER --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="spi.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


