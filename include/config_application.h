/*
 * Femto OS v 0.92 - Copyright (C) 2008-2010 Ruud Vlaming
 *
 * This file is part of the Femto OS distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Please note that, due to the GPLv3 license, for application of this
 * work and/or combined work in embedded systems special obligations apply.
 * If these are not to you liking, please know the Femto OS is dual
 * licensed. A commercial license and support are available.
 * See http://www.femtoos.org/ for details.
 */

#ifndef CONFIG_RECORDCLEAN_H_
#define CONFIG_RECORDCLEAN_H_

/* Comments are in the config default file */

/* ========================================================================= */
/* SYSTEM CONFIGURATION ==================================================== */
/* ========================================================================= */

#define  cfgSysOptimized                         cfgFalse
#ifdef DEBUG
#define  cfgSysDebug                             cfgFalse
#define  cfgSysShowErrorOnUart                   cfgTrue
#else
#define  cfgSysDebug                             cfgFalse
#define  cfgSysShowErrorOnUart                   cfgFalse
#endif
#define  cfgSysInline                            cfgTrue
#define  cfgSysFramePointerCounterMeasures       cfgFalse
#define  cfgSysReduceProEpilogue                 cfgBikini
#define  cfgSysRegisterCheckByte                 0xA5
#define  cfgSysStackGrowthUp                     cfgFalse
#define  cfgSysZeroPageStack                     cfgFalse
#define  cfgSysClearUnusedR1                     cfgTrue
#define  cfgSysSqueezeState                      defSysOptimized //changes made in femtoos_ATmega8535.asm
#define  cfgSysReuseOsStack                      cfgTrue
#define  cfgSysClockDivider                      1 	// This Clock and SubTick
#define  cfgSysSubTicksPerFullTick               32 // values equal to Tick
#define  cfgSysSubTickDivider                    64 // frequency of about 2,6 KHz (@ 8 MHz)
#define  cfgSysGCCstartup                        cfgKeep
#define  cfgSysFsStorageSpace                    0

/* ========================================================================= */
/* INTERRUPT HANDLING ====================================================== */
/* ========================================================================= */

#define  cfgIntGlobalOnly                        cfgTrue
#define  cfgIntUserDefined                       cfgTrue
#define  cfgIntSwitchUsesOSstack                 cfgFalse
#define  cfgIntOsProtected                       cfgTrue
#define  cfgIntTickTrack                         cfgFalse
#define  cfgIntManualTicks                       cfgFalse


/* ========================================================================= */
/* QUALITY ASSURANCE ======================================================= */
/* ========================================================================= */

#define  cfgCheckSystem                          cfgTrue
#define  cfgCheckReset                           cfgFalse
#define  cfgCheckTrace                           cfgFalse
#define  cfgCheckAlwaysFatal                     cfgFalse
#define  cfgCheckWatermarks                      cfgFalse
#define  cfgCheckOsStack                         cfgTrue
#define  cfgCheckTaskStack                       cfgTrue
#define  cfgCheckIsrStack                        cfgFalse
#define  cfgCheckRegisters                       cfgTrue
#define  cfgCheckTiming                          cfgFalse
#define  cfgCheckQueuFilling                     cfgFalse
#define  cfgCheckMethodUse                       cfgTrue
#define  cfgCheckApplication                     cfgFalse


/* ========================================================================= */
/* COMMON CAPABILITIES ===================================================== */
/* ========================================================================= */

#define  cfgUseEquidistantTicks                  cfgTrue
#define  cfgUseDelay                             cfgTrue
#define  cfgUseSuspendOnDelay                    cfgFalse
#define  cfgUseSynchronization                   cfgSyncNon
#define  cfgUseHierarchicalRoundRobin            cfgFalse
#define  cfgUseNestedCriticals                   cfgFalse
#define  cfgUsePrioritizedRelease                cfgFalse
#define  cfgUsePriorityLifting                   cfgFalse
#define  cfgUseTaskWatchdog                      cfgFalse
#define  cfgUseCorrectWakeupTimes                cfgFalse
#define  cfgUseTimeout                           cfgFalse
#define  cfgUseTasknames                         cfgFalse
#define  cfgUseLowPowerSleep                     cfgFalse
#define  cfgUseLowPowerOnDelay                   cfgFalse
#define  cfgUseLowPowerDelayRelease              cfgFalse
#define  cfgUseLoadMonitor                       cfgFalse
#define  cfgUseFileSystem                        cfgFalse
#define  cfgUseFileSystemConcurrentRead          cfgFalse
#define  cfgUseFileSystemEconomyMode             cfgFalse
#define  cfgUseFileSystemMaintainFAT             cfgFalse
#define  cfgUseEvents                            cfgFalse
#define  cfgUseEventsOnVariables                 cfgFalse


/* ========================================================================= */
/* CODE PARAMETERS ========================================================= */
/* ========================================================================= */

#define  cfgNumWatchdogDiv                       0
#define  cfgNumMonitorDiv                        0
#define  cfgNumSleepPeriod                       0
#define  cfgNumSleepThreshold                    0
#define  cfgNumGlobWidth                         0
#define  cfgNumSwitchWidth                       0
#define  cfgNumTickWidth                         0


/* ========================================================================= */
/* EXTERNAL CALLS ========================================================== */
/* ========================================================================= */

#define  callAppTick00                           cfgTrue
#define  callAppTick08                           cfgFalse
#define  callAppTick16                           cfgFalse
#define  callAppTickSleep                        cfgFalse
#define  callAppEnterIdle                        cfgFalse
#define  callAppEnterSleep                       cfgFalse
#define  callAppExitSleep                        cfgFalse
#define  callAppBoot                             cfgTrue
#define  callAppInit                             cfgFalse
#define  callAppBark                             cfgFalse


/* ========================================================================= */
/* API INCLUSIONS ========================================================== */
/* ========================================================================= */

/* Standard functions ------------------------------------------------------ */
#define  includeTaskYield                        cfgFalse
#define  includeTaskDelayFromNow                 cfgTrue
#define  includeTaskDelayFromWake                cfgTrue
#define  includeGenSuspend                       cfgFalse
#define  includeTaskSuspend                      cfgFalse
#define  includeGenResume                        cfgFalse
#define  includeTaskSleep                        cfgFalse
#define  includeTaskSleepAll                     cfgFalse
#define  includeGenSetPriority                   cfgFalse
#define  includeGenGetPriority                   cfgFalse
#define  includeGenGetTickCount                  cfgFalse
#define  includeGenAddtoTickCount                cfgFalse
#define  includeGenGetLastWakeTime               cfgFalse
#define  includeTaskFeedWatchdog                 cfgFalse
#define  includeTaskKillWatchdog                 cfgFalse
#define  includeGenGetTaskname                   cfgFalse
#define  includeTaskTerminate                    cfgFalse
#define  includeGenReboot                        cfgFalse
#define  includeTaskRecreate                     cfgFalse
#define  includeTaskRestart                      cfgFalse
#define  includeGenLogTask                       cfgFalse
#define  includeGenLogOs                         cfgFalse
#define  includeGenTrace                         cfgFalse
#define  includeGenPipeInt16                     cfgFalse
#define  includeGenPassFlashString               cfgFalse

/* Event functions --------------------------------------------------------- */
#define  includeTaskWaitForEvents                cfgFalse
#define  includeIsrFireEvent                     cfgFalse
#define  includeGenFireEvent                     cfgFalse
#define  includeGenFireEventSet                  cfgFalse
#define  includeGenCountEventBlocks              cfgFalse

/* Synchronization functions ----------------------------------------------- */
#define  includeTaskWaitForTasks                 cfgFalse
#define  includeGenWaitRelease                   cfgFalse
#define  includeTaskMutex                        cfgFalse
#define  includeTaskQueu                         cfgFalse

/* Queue functions --------------------------------------------------------- */
#define  includeGenQueuWrite                     cfgFalse
#define  includeGenQueuRead                      cfgFalse
#define  includeGenQueuUnwrite                   cfgFalse
#define  includeGenQueuUnread                    cfgFalse
#define  includeGenQueuClear                     cfgFalse
#define  includeGenQueuPeek                      cfgFalse
#define  includeGenQueuReadable                  cfgFalse
#define  includeGenQueuWriteable                 cfgFalse
#define  includeGenQueuFull                      cfgFalse
#define  includeGenQueuEmpty                     cfgFalse

/* File System functions --------------------------------------------------- */
#define  includeTaskFileAccess                   cfgFalse
#define  includeTaskFileFormat                   cfgFalse
#define  includeTaskFileGetSize                  cfgFalse
#define  includeTaskFileSetSize                  cfgFalse
#define  includeTaskFileReadByte                 cfgFalse
#define  includeTaskFileWriteByte                cfgFalse
#define  includeTaskFileAppendByte               cfgFalse
#define  includeTaskFileReadPipe                 cfgFalse
#define  includeTaskFileWritePipe                cfgFalse
#define  includeTaskFileReadBuffer               cfgFalse
#define  includeTaskFileWriteBuffer              cfgFalse

/* Protection functions ---------------------------------------------------- */
#define  includeTaskProtectGlobalInterrupts      cfgFalse
#define  includeTaskProtectSwitchTasks           cfgFalse
#define  includeTaskProtectTickInterrupts        cfgFalse
#define  includeTaskProtectGlobalCritical        cfgTrue
#define  includeTaskProtectSwitchCritical        cfgFalse
#define  includeTaskProtectTickCritical          cfgFalse

/* ISR header/footer functions --------------------------------------------- */
#define  includeIsrEnter                         cfgTrue
#define  includeIsrExit                          cfgTrue
#define  includeIsrBegin                         cfgFalse
#define  includeIsrEndReturn                     cfgFalse
#define  includeIsrEndYield                      cfgFalse
#define  includeIsrStartLoad                     cfgFalse
#define  includeIsrStopLoad                      cfgFalse

/* Quality assurance functions --------------------------------------------- */
#define  includeTaskStackCheck                   cfgFalse
#define  includeIsrStackCheck                    cfgFalse


/* ========================================================================= */
/* TASK NAMES ============================================================== */
/* ========================================================================= */

#define  CN_00                                   Servo
#define  CN_01                                   RecordClean
#define  CN_02                                   Background


/* ========================================================================= */
/* INCLUDE TASKS =========================================================== */
/* ========================================================================= */

#define  TaskInclude_Servo                       cfgStartRunning
#define  TaskInclude_RecordClean                 cfgStartRunning
#define  TaskInclude_Background                  cfgStartRunning


/* ========================================================================= */
/* TASK CAPABILITIES ======================================================= */
/* ========================================================================= */

#define  CapabilitiesOverride                    cfgCapAll


/* ========================================================================= */
/* STACK SIZES ============================================================= */
/* ========================================================================= */

#define  StackSafety                             4
#define  StackSizeOS                             32
#define  StackSizeISR                            32
#define  StackSizeShared                         0
#define  StackSizeOverride                       48


/* ========================================================================= */
/* TIMES SLICES ============================================================ */
/* ========================================================================= */

#define  TimeSliceIdleTime                       cfgSysSubTicksPerFullTick


/* ========================================================================= */
/* RUNTIME TASK NAMES ====================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* TASK PRIORITIES ========================================================= */
/* ========================================================================= */

#define  PriorityOverride                        cfgOverrideNon

#define  Priority_Servo                          1
#define  Priority_RecordClean                    1
#define  Priority_Background                     0


/* ========================================================================= */
/* REGISTER USE ============================================================ */
/* ========================================================================= */

#define  RegisterUse_Servo                       registersAll
#define  RegisterUse_RecordClean                 registersAll
#define  RegisterUse_Background					 registersAll

/* ========================================================================= */
/* REGISTER CHECK ========================================================== */
/* ========================================================================= */

#define  RegisterCheckOverride                   registersAll


/* ========================================================================= */
/* INTERRUPT SETTING ======================================================= */
/* ========================================================================= */

#define  InterruptStartOverride                  cfgGlobSet | cfgTickSet


/* ========================================================================= */
/* EVENT NAMES ============================================================= */
/* ========================================================================= */


/* ========================================================================= */
/* SLOT NAMES ============================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* SLOT SIZES ============================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* SLOT USAGE ============================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* QUEU SIZES ============================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* FILE NAMES ============================================================== */
/* ========================================================================= */


/* ========================================================================= */
/* FILE SIZES ============================================================== */
/* ========================================================================= */

#define  FileSpaceStandard                       0


#endif /*CONFIG_RECORDCLEAN_H_*/
