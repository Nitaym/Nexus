#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1577468939/CFileComm.o \
	${OBJECTDIR}/_ext/1577468939/CLoopbackComm.o \
	${OBJECTDIR}/_ext/1654212647/ICommLayer.o \
	${OBJECTDIR}/_ext/1577468939/CConsoleComm.o \
	${OBJECTDIR}/_ext/1577468939/CClientSocket.o \
	${OBJECTDIR}/_ext/1577468939/CUart.o \
	${OBJECTDIR}/_ext/1290415847/Utils.o \
	${OBJECTDIR}/_ext/1654212647/CAutoConnect.o \
	${OBJECTDIR}/_ext/677239803/CDebug.o \
	${OBJECTDIR}/_ext/677239803/CData.o \
	${OBJECTDIR}/_ext/1654212647/CAsyncReceiver.o \
	${OBJECTDIR}/_ext/1654212647/TComm.o \
	${OBJECTDIR}/_ext/1577468939/CLoggerComm.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnexus.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnexus.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnexus.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnexus.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnexus.a

${OBJECTDIR}/_ext/1577468939/CFileComm.o: ../../../Comm/Physical/CFileComm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1577468939
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1577468939/CFileComm.o ../../../Comm/Physical/CFileComm.cpp

${OBJECTDIR}/_ext/1577468939/CLoopbackComm.o: ../../../Comm/Physical/CLoopbackComm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1577468939
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1577468939/CLoopbackComm.o ../../../Comm/Physical/CLoopbackComm.cpp

${OBJECTDIR}/_ext/1654212647/ICommLayer.o: ../../../Comm/Protocols/ICommLayer.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1654212647
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1654212647/ICommLayer.o ../../../Comm/Protocols/ICommLayer.cpp

${OBJECTDIR}/_ext/1577468939/CConsoleComm.o: ../../../Comm/Physical/CConsoleComm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1577468939
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1577468939/CConsoleComm.o ../../../Comm/Physical/CConsoleComm.cpp

${OBJECTDIR}/_ext/1577468939/CClientSocket.o: ../../../Comm/Physical/CClientSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1577468939
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1577468939/CClientSocket.o ../../../Comm/Physical/CClientSocket.cpp

${OBJECTDIR}/_ext/1577468939/CUart.o: ../../../Comm/Physical/CUart.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1577468939
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1577468939/CUart.o ../../../Comm/Physical/CUart.cpp

${OBJECTDIR}/_ext/1290415847/Utils.o: ../../../General/Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1290415847
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1290415847/Utils.o ../../../General/Utils.cpp

${OBJECTDIR}/_ext/1654212647/CAutoConnect.o: ../../../Comm/Protocols/CAutoConnect.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1654212647
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1654212647/CAutoConnect.o ../../../Comm/Protocols/CAutoConnect.cpp

${OBJECTDIR}/_ext/677239803/CDebug.o: ../../../Comm/Base/CDebug.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/677239803
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/677239803/CDebug.o ../../../Comm/Base/CDebug.cpp

${OBJECTDIR}/_ext/677239803/CData.o: ../../../Comm/Base/CData.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/677239803
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/677239803/CData.o ../../../Comm/Base/CData.cpp

${OBJECTDIR}/_ext/1654212647/CAsyncReceiver.o: ../../../Comm/Protocols/CAsyncReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1654212647
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1654212647/CAsyncReceiver.o ../../../Comm/Protocols/CAsyncReceiver.cpp

${OBJECTDIR}/_ext/1654212647/TComm.o: ../../../Comm/Protocols/TComm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1654212647
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1654212647/TComm.o ../../../Comm/Protocols/TComm.cpp

${OBJECTDIR}/_ext/1577468939/CLoggerComm.o: ../../../Comm/Physical/CLoggerComm.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1577468939
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../ -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1577468939/CLoggerComm.o ../../../Comm/Physical/CLoggerComm.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnexus.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
