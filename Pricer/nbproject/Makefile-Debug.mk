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
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AsianOption.o \
	${OBJECTDIR}/BasketOption.o \
	${OBJECTDIR}/BlackScholesModel.o \
	${OBJECTDIR}/MonteCarlo.o \
	${OBJECTDIR}/Option.o \
	${OBJECTDIR}/PerformanceOption.o \
	${OBJECTDIR}/TestAsian.o \
	${OBJECTDIR}/TestPerformance.o \
	${OBJECTDIR}/Tests/TestBlackScholes.o \
	${OBJECTDIR}/Tests/TestBlackScholes2.o \
	${OBJECTDIR}/Tests/TestMonteCarloZero.o \
	${OBJECTDIR}/Tests/TestMonteCarloZero2.o \
	${OBJECTDIR}/mc_example.o \
	${OBJECTDIR}/parser.o \
	${OBJECTDIR}/testBasket.o \
	${OBJECTDIR}/test_parser.o


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
LDLIBSOPTIONS=-L/matieres/5MMPMP6/pnl/lib -lpnl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/projetmodelisation

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/projetmodelisation: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/projetmodelisation ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/AsianOption.o: AsianOption.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AsianOption.o AsianOption.cpp

${OBJECTDIR}/BasketOption.o: BasketOption.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BasketOption.o BasketOption.cpp

${OBJECTDIR}/BlackScholesModel.o: BlackScholesModel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BlackScholesModel.o BlackScholesModel.cpp

${OBJECTDIR}/MonteCarlo.o: MonteCarlo.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MonteCarlo.o MonteCarlo.cpp

${OBJECTDIR}/Option.o: Option.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Option.o Option.cpp

${OBJECTDIR}/PerformanceOption.o: PerformanceOption.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/PerformanceOption.o PerformanceOption.cpp

${OBJECTDIR}/TestAsian.o: TestAsian.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TestAsian.o TestAsian.cpp

${OBJECTDIR}/TestPerformance.o: TestPerformance.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TestPerformance.o TestPerformance.cpp

${OBJECTDIR}/Tests/TestBlackScholes.o: Tests/TestBlackScholes.cpp 
	${MKDIR} -p ${OBJECTDIR}/Tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Tests/TestBlackScholes.o Tests/TestBlackScholes.cpp

${OBJECTDIR}/Tests/TestBlackScholes2.o: Tests/TestBlackScholes2.cpp 
	${MKDIR} -p ${OBJECTDIR}/Tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Tests/TestBlackScholes2.o Tests/TestBlackScholes2.cpp

${OBJECTDIR}/Tests/TestMonteCarloZero.o: Tests/TestMonteCarloZero.cpp 
	${MKDIR} -p ${OBJECTDIR}/Tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Tests/TestMonteCarloZero.o Tests/TestMonteCarloZero.cpp

${OBJECTDIR}/Tests/TestMonteCarloZero2.o: Tests/TestMonteCarloZero2.cpp 
	${MKDIR} -p ${OBJECTDIR}/Tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Tests/TestMonteCarloZero2.o Tests/TestMonteCarloZero2.cpp

${OBJECTDIR}/mc_example.o: mc_example.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mc_example.o mc_example.cpp

${OBJECTDIR}/parser.o: parser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/parser.o parser.cpp

${OBJECTDIR}/testBasket.o: testBasket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/testBasket.o testBasket.cpp

${OBJECTDIR}/test_parser.o: test_parser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/matieres/5MMPMP6/pnl/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/test_parser.o test_parser.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/projetmodelisation

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
