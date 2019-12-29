################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
PatchProgrammer_DR_Patch.obj: C:/ti/PatchProgrammerMSP430FR5739-1.14/Patch\ Programmer\ Source/Source/Patch\ Programmer/PatchProgrammer_DR_Patch.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O0 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/PatchProgrammerMSP430FR5739-1.14/Patch Programmer Source/Source/CC3000HostDriver" --include_path="C:/ti/PatchProgrammerMSP430FR5739-1.14/Patch Programmer Source/Source/inc" --include_path="C:/ti/PatchProgrammerMSP430FR5739-1.14/Patch Programmer Source/Source/CC3000 Spi" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=__CCS__ --define=CC3000_TINY_DRIVER --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="PatchProgrammer_DR_Patch.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

board.obj: C:/ti/PatchProgrammerMSP430FR5739-1.14/Patch\ Programmer\ Source/Source/Patch\ Programmer/board.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"c:/ti/ccsv6/tools/compiler/msp430_4.3.1/bin/cl430" -vmspx --abi=coffabi -O0 --opt_for_speed=0 -g --include_path="c:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/PatchProgrammerMSP430FR5739-1.14/Patch Programmer Source/Source/CC3000HostDriver" --include_path="C:/ti/PatchProgrammerMSP430FR5739-1.14/Patch Programmer Source/Source/inc" --include_path="C:/ti/PatchProgrammerMSP430FR5739-1.14/Patch Programmer Source/Source/CC3000 Spi" --include_path="c:/ti/ccsv6/tools/compiler/msp430_4.3.1/include" --gcc --define=__CCS__ --define=CC3000_TINY_DRIVER --define=CC3000_UNENCRYPTED_SMART_CONFIG --define=__MSP430FR5739__ --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="board.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


