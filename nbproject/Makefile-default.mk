#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/cbuffer.c src/frame.c src/functions.c src/isr.c src/main.c src/main_old.c src/setup.c src/uartbuf.c src/utils.c src/myfunc/spi.c src/myfunc/Tsensor.c src/myfunc/wr_eeprom.c src/myfunc/dac8568.c src/myfunc/analog.c src/myfunc/maths.c src/myfunc/ads8332.c src/myfunc/display.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/cbuffer.p1 ${OBJECTDIR}/src/frame.p1 ${OBJECTDIR}/src/functions.p1 ${OBJECTDIR}/src/isr.p1 ${OBJECTDIR}/src/main.p1 ${OBJECTDIR}/src/main_old.p1 ${OBJECTDIR}/src/setup.p1 ${OBJECTDIR}/src/uartbuf.p1 ${OBJECTDIR}/src/utils.p1 ${OBJECTDIR}/src/myfunc/spi.p1 ${OBJECTDIR}/src/myfunc/Tsensor.p1 ${OBJECTDIR}/src/myfunc/wr_eeprom.p1 ${OBJECTDIR}/src/myfunc/dac8568.p1 ${OBJECTDIR}/src/myfunc/analog.p1 ${OBJECTDIR}/src/myfunc/maths.p1 ${OBJECTDIR}/src/myfunc/ads8332.p1 ${OBJECTDIR}/src/myfunc/display.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/src/cbuffer.p1.d ${OBJECTDIR}/src/frame.p1.d ${OBJECTDIR}/src/functions.p1.d ${OBJECTDIR}/src/isr.p1.d ${OBJECTDIR}/src/main.p1.d ${OBJECTDIR}/src/main_old.p1.d ${OBJECTDIR}/src/setup.p1.d ${OBJECTDIR}/src/uartbuf.p1.d ${OBJECTDIR}/src/utils.p1.d ${OBJECTDIR}/src/myfunc/spi.p1.d ${OBJECTDIR}/src/myfunc/Tsensor.p1.d ${OBJECTDIR}/src/myfunc/wr_eeprom.p1.d ${OBJECTDIR}/src/myfunc/dac8568.p1.d ${OBJECTDIR}/src/myfunc/analog.p1.d ${OBJECTDIR}/src/myfunc/maths.p1.d ${OBJECTDIR}/src/myfunc/ads8332.p1.d ${OBJECTDIR}/src/myfunc/display.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/cbuffer.p1 ${OBJECTDIR}/src/frame.p1 ${OBJECTDIR}/src/functions.p1 ${OBJECTDIR}/src/isr.p1 ${OBJECTDIR}/src/main.p1 ${OBJECTDIR}/src/main_old.p1 ${OBJECTDIR}/src/setup.p1 ${OBJECTDIR}/src/uartbuf.p1 ${OBJECTDIR}/src/utils.p1 ${OBJECTDIR}/src/myfunc/spi.p1 ${OBJECTDIR}/src/myfunc/Tsensor.p1 ${OBJECTDIR}/src/myfunc/wr_eeprom.p1 ${OBJECTDIR}/src/myfunc/dac8568.p1 ${OBJECTDIR}/src/myfunc/analog.p1 ${OBJECTDIR}/src/myfunc/maths.p1 ${OBJECTDIR}/src/myfunc/ads8332.p1 ${OBJECTDIR}/src/myfunc/display.p1

# Source Files
SOURCEFILES=src/cbuffer.c src/frame.c src/functions.c src/isr.c src/main.c src/main_old.c src/setup.c src/uartbuf.c src/utils.c src/myfunc/spi.c src/myfunc/Tsensor.c src/myfunc/wr_eeprom.c src/myfunc/dac8568.c src/myfunc/analog.c src/myfunc/maths.c src/myfunc/ads8332.c src/myfunc/display.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F46K20
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/cbuffer.p1: src/cbuffer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cbuffer.p1.d 
	@${RM} ${OBJECTDIR}/src/cbuffer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/cbuffer.p1 src/cbuffer.c 
	@-${MV} ${OBJECTDIR}/src/cbuffer.d ${OBJECTDIR}/src/cbuffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/cbuffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/frame.p1: src/frame.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/frame.p1.d 
	@${RM} ${OBJECTDIR}/src/frame.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/frame.p1 src/frame.c 
	@-${MV} ${OBJECTDIR}/src/frame.d ${OBJECTDIR}/src/frame.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/frame.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/functions.p1: src/functions.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/functions.p1.d 
	@${RM} ${OBJECTDIR}/src/functions.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/functions.p1 src/functions.c 
	@-${MV} ${OBJECTDIR}/src/functions.d ${OBJECTDIR}/src/functions.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/functions.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/isr.p1: src/isr.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/isr.p1.d 
	@${RM} ${OBJECTDIR}/src/isr.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/isr.p1 src/isr.c 
	@-${MV} ${OBJECTDIR}/src/isr.d ${OBJECTDIR}/src/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/main_old.p1: src/main_old.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main_old.p1.d 
	@${RM} ${OBJECTDIR}/src/main_old.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/main_old.p1 src/main_old.c 
	@-${MV} ${OBJECTDIR}/src/main_old.d ${OBJECTDIR}/src/main_old.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main_old.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/setup.p1: src/setup.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/setup.p1.d 
	@${RM} ${OBJECTDIR}/src/setup.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/setup.p1 src/setup.c 
	@-${MV} ${OBJECTDIR}/src/setup.d ${OBJECTDIR}/src/setup.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/setup.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/uartbuf.p1: src/uartbuf.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uartbuf.p1.d 
	@${RM} ${OBJECTDIR}/src/uartbuf.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/uartbuf.p1 src/uartbuf.c 
	@-${MV} ${OBJECTDIR}/src/uartbuf.d ${OBJECTDIR}/src/uartbuf.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/uartbuf.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/utils.p1: src/utils.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/utils.p1.d 
	@${RM} ${OBJECTDIR}/src/utils.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/utils.p1 src/utils.c 
	@-${MV} ${OBJECTDIR}/src/utils.d ${OBJECTDIR}/src/utils.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/utils.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/spi.p1: src/myfunc/spi.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/spi.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/spi.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/spi.p1 src/myfunc/spi.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/spi.d ${OBJECTDIR}/src/myfunc/spi.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/spi.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/Tsensor.p1: src/myfunc/Tsensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/Tsensor.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/Tsensor.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/Tsensor.p1 src/myfunc/Tsensor.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/Tsensor.d ${OBJECTDIR}/src/myfunc/Tsensor.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/Tsensor.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/wr_eeprom.p1: src/myfunc/wr_eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/wr_eeprom.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/wr_eeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/wr_eeprom.p1 src/myfunc/wr_eeprom.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/wr_eeprom.d ${OBJECTDIR}/src/myfunc/wr_eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/wr_eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/dac8568.p1: src/myfunc/dac8568.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/dac8568.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/dac8568.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/dac8568.p1 src/myfunc/dac8568.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/dac8568.d ${OBJECTDIR}/src/myfunc/dac8568.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/dac8568.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/analog.p1: src/myfunc/analog.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/analog.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/analog.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/analog.p1 src/myfunc/analog.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/analog.d ${OBJECTDIR}/src/myfunc/analog.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/analog.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/maths.p1: src/myfunc/maths.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/maths.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/maths.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/maths.p1 src/myfunc/maths.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/maths.d ${OBJECTDIR}/src/myfunc/maths.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/maths.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/ads8332.p1: src/myfunc/ads8332.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/ads8332.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/ads8332.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/ads8332.p1 src/myfunc/ads8332.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/ads8332.d ${OBJECTDIR}/src/myfunc/ads8332.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/ads8332.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/display.p1: src/myfunc/display.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/display.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/display.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/display.p1 src/myfunc/display.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/display.d ${OBJECTDIR}/src/myfunc/display.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/display.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/cbuffer.p1: src/cbuffer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cbuffer.p1.d 
	@${RM} ${OBJECTDIR}/src/cbuffer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/cbuffer.p1 src/cbuffer.c 
	@-${MV} ${OBJECTDIR}/src/cbuffer.d ${OBJECTDIR}/src/cbuffer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/cbuffer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/frame.p1: src/frame.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/frame.p1.d 
	@${RM} ${OBJECTDIR}/src/frame.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/frame.p1 src/frame.c 
	@-${MV} ${OBJECTDIR}/src/frame.d ${OBJECTDIR}/src/frame.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/frame.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/functions.p1: src/functions.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/functions.p1.d 
	@${RM} ${OBJECTDIR}/src/functions.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/functions.p1 src/functions.c 
	@-${MV} ${OBJECTDIR}/src/functions.d ${OBJECTDIR}/src/functions.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/functions.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/isr.p1: src/isr.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/isr.p1.d 
	@${RM} ${OBJECTDIR}/src/isr.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/isr.p1 src/isr.c 
	@-${MV} ${OBJECTDIR}/src/isr.d ${OBJECTDIR}/src/isr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/isr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/main.p1: src/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.p1.d 
	@${RM} ${OBJECTDIR}/src/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/main.p1 src/main.c 
	@-${MV} ${OBJECTDIR}/src/main.d ${OBJECTDIR}/src/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/main_old.p1: src/main_old.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main_old.p1.d 
	@${RM} ${OBJECTDIR}/src/main_old.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/main_old.p1 src/main_old.c 
	@-${MV} ${OBJECTDIR}/src/main_old.d ${OBJECTDIR}/src/main_old.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/main_old.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/setup.p1: src/setup.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/setup.p1.d 
	@${RM} ${OBJECTDIR}/src/setup.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/setup.p1 src/setup.c 
	@-${MV} ${OBJECTDIR}/src/setup.d ${OBJECTDIR}/src/setup.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/setup.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/uartbuf.p1: src/uartbuf.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uartbuf.p1.d 
	@${RM} ${OBJECTDIR}/src/uartbuf.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/uartbuf.p1 src/uartbuf.c 
	@-${MV} ${OBJECTDIR}/src/uartbuf.d ${OBJECTDIR}/src/uartbuf.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/uartbuf.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/utils.p1: src/utils.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/utils.p1.d 
	@${RM} ${OBJECTDIR}/src/utils.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/utils.p1 src/utils.c 
	@-${MV} ${OBJECTDIR}/src/utils.d ${OBJECTDIR}/src/utils.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/utils.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/spi.p1: src/myfunc/spi.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/spi.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/spi.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/spi.p1 src/myfunc/spi.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/spi.d ${OBJECTDIR}/src/myfunc/spi.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/spi.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/Tsensor.p1: src/myfunc/Tsensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/Tsensor.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/Tsensor.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/Tsensor.p1 src/myfunc/Tsensor.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/Tsensor.d ${OBJECTDIR}/src/myfunc/Tsensor.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/Tsensor.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/wr_eeprom.p1: src/myfunc/wr_eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/wr_eeprom.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/wr_eeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/wr_eeprom.p1 src/myfunc/wr_eeprom.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/wr_eeprom.d ${OBJECTDIR}/src/myfunc/wr_eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/wr_eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/dac8568.p1: src/myfunc/dac8568.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/dac8568.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/dac8568.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/dac8568.p1 src/myfunc/dac8568.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/dac8568.d ${OBJECTDIR}/src/myfunc/dac8568.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/dac8568.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/analog.p1: src/myfunc/analog.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/analog.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/analog.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/analog.p1 src/myfunc/analog.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/analog.d ${OBJECTDIR}/src/myfunc/analog.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/analog.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/maths.p1: src/myfunc/maths.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/maths.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/maths.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/maths.p1 src/myfunc/maths.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/maths.d ${OBJECTDIR}/src/myfunc/maths.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/maths.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/ads8332.p1: src/myfunc/ads8332.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/ads8332.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/ads8332.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/ads8332.p1 src/myfunc/ads8332.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/ads8332.d ${OBJECTDIR}/src/myfunc/ads8332.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/ads8332.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/myfunc/display.p1: src/myfunc/display.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src/myfunc" 
	@${RM} ${OBJECTDIR}/src/myfunc/display.p1.d 
	@${RM} ${OBJECTDIR}/src/myfunc/display.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/src/myfunc/display.p1 src/myfunc/display.c 
	@-${MV} ${OBJECTDIR}/src/myfunc/display.d ${OBJECTDIR}/src/myfunc/display.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/myfunc/display.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=none  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/fazia-pic_ModernXC8.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
