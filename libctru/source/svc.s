#include <3ds/asminc.h>

.arm
.align 4

.macro SVC_BEGIN name
	BEGIN_ASM_FUNC \name
.endm

.macro SVC_END
	END_ASM_FUNC
.endm

SVC_BEGIN svcControlMemory
	push {r0, r4}
	ldr  r0, [sp, #0x8]
	ldr  r4, [sp, #0x8+0x4]
	svc  0x01
	ldr  r2, [sp], #4
	str  r1, [r2]
	ldr  r4, [sp], #4
	bx   lr
SVC_END

SVC_BEGIN svcQueryMemory
	push {r0, r1, r4-r6}
	svc  0x02
	ldr  r6, [sp]
	str  r1, [r6]
	str  r2, [r6, #4]
	str  r3, [r6, #8]
	str  r4, [r6, #0xc]
	ldr  r6, [sp, #4]
	str  r5, [r6]
	add  sp, sp, #8
	pop  {r4-r6}
	bx   lr
SVC_END

SVC_BEGIN svcExitProcess
	svc 0x03
	bx  lr
SVC_END

SVC_BEGIN svcGetProcessAffinityMask
	svc 0x04
	bx lr
SVC_END

SVC_BEGIN svcSetProcessAffinityMask
	svc 0x05
	bx lr
SVC_END

SVC_BEGIN svcGetProcessIdealProcessor
	str r0, [sp, #-0x4]!
	svc 0x06
	ldr r3, [sp], #4
	str r1, [r3]
	bx lr
SVC_END

SVC_BEGIN svcSetProcessIdealProcessor
	svc 0x07
	bx lr
SVC_END

SVC_BEGIN svcCreateThread
	push {r0, r4}
	ldr  r0, [sp, #0x8]
	ldr  r4, [sp, #0x8+0x4]
	svc  0x08
	ldr  r2, [sp], #4
	str  r1, [r2]
	ldr  r4, [sp], #4
	bx   lr
SVC_END

SVC_BEGIN svcExitThread
	svc 0x09
	bx  lr
SVC_END

SVC_BEGIN svcSleepThread
	svc 0x0A
	bx  lr
SVC_END

SVC_BEGIN svcGetThreadPriority
	str r0, [sp, #-0x4]!
	svc 0x0B
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcSetThreadPriority
	svc 0x0C
	bx  lr
SVC_END

SVC_BEGIN svcGetThreadAffinityMask
	svc 0x0D
	bx  lr
SVC_END

SVC_BEGIN svcSetThreadAffinityMask
	svc 0x0E
	bx  lr
SVC_END

SVC_BEGIN svcGetThreadIdealProcessor
	str r0, [sp, #-0x4]!
	svc 0x0F
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcSetThreadIdealProcessor
	svc 0x10
	bx  lr
SVC_END

SVC_BEGIN svcGetProcessorID
	svc 0x11
	bx  lr
SVC_END

SVC_BEGIN svcRun
	push {r4,r5}
	ldr r2, [r1, #0x04]
	ldr r3, [r1, #0x08]
	ldr r4, [r1, #0x0C]
	ldr r5, [r1, #0x10]
	ldr r1, [r1, #0x00]
	svc 0x12
	pop {r4,r5}
	bx  lr
SVC_END

SVC_BEGIN svcCreateMutex
	str r0, [sp, #-4]!
	svc 0x13
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcReleaseMutex
	svc 0x14
	bx  lr
SVC_END

SVC_BEGIN svcCreateSemaphore
	push {r0}
	svc 0x15
	pop {r3}
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcReleaseSemaphore
	push {r0}
	svc  0x16
	pop  {r3}
	str  r1, [r3]
	bx   lr
SVC_END

SVC_BEGIN svcCreateEvent
	str r0, [sp, #-4]!
	svc 0x17
	ldr r2, [sp], #4
	str r1, [r2]
	bx  lr
SVC_END

SVC_BEGIN svcSignalEvent
	svc 0x18
	bx  lr
SVC_END

SVC_BEGIN svcClearEvent
	svc 0x19
	bx  lr
SVC_END

SVC_BEGIN svcCreateTimer
	str r0, [sp, #-4]!
	svc 0x1A
	ldr r2, [sp], #4
	str r1, [r2]
	bx  lr
SVC_END

SVC_BEGIN svcSetTimer
	str r4, [sp, #-4]!
	ldr r1, [sp, #4]
	ldr r4, [sp, #8]
	svc 0x1B
	ldr r4, [sp], #4
	bx  lr
SVC_END

SVC_BEGIN svcCancelTimer
	svc 0x1C
	bx  lr
SVC_END

SVC_BEGIN svcClearTimer
	svc 0x1D
	bx  lr
SVC_END

SVC_BEGIN svcCreateMemoryBlock
	str r0, [sp, #-4]!
	ldr r0, [sp, #4]
	svc 0x1E
	ldr r2, [sp], #4
	str r1, [r2]
	bx  lr
SVC_END

SVC_BEGIN svcMapMemoryBlock
	svc 0x1F
	bx  lr
SVC_END

SVC_BEGIN svcUnmapMemoryBlock
	svc 0x20
	bx  lr
SVC_END

SVC_BEGIN svcCreateAddressArbiter
	push {r0}
	svc 0x21
	pop {r2}
	str r1, [r2]
	bx  lr
SVC_END

SVC_BEGIN svcArbitrateAddress
	push {r4, r5}
	ldr r4, [sp, #8]
	ldr r5, [sp, #12]
	svc 0x22
	pop {r4, r5}
	bx  lr
SVC_END

SVC_BEGIN svcArbitrateAddressNoTimeout
	svc 0x22
	bx  lr
SVC_END

SVC_BEGIN svcCloseHandle
	svc 0x23
	bx  lr
SVC_END

SVC_BEGIN svcWaitSynchronization
	svc 0x24
	bx  lr
SVC_END

SVC_BEGIN svcWaitSynchronizationN
	str r5, [sp, #-4]!
	str r4, [sp, #-4]!
	mov r5, r0
	ldr r0, [sp, #0x8]
	ldr r4, [sp, #0x8+0x4]
	svc 0x25
	str r1, [r5]
	ldr r4, [sp], #4
	ldr r5, [sp], #4
	bx  lr
SVC_END

SVC_BEGIN svcDuplicateHandle
	str r0, [sp, #-0x4]!
	svc 0x27
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcGetSystemTick
	svc 0x28
	bx  lr
SVC_END

SVC_BEGIN svcGetHandleInfo
	str r0, [sp, #-0x4]!
	svc 0x29
	ldr r3, [sp], #4
	str r1, [r3]
	str r2, [r3,#4]
	bx lr
SVC_END

SVC_BEGIN svcGetSystemInfo
	str r0, [sp, #-0x4]!
	svc 0x2A
	ldr r3, [sp], #4
	str r1, [r3]
	str r2, [r3,#4]
	bx  lr
SVC_END

SVC_BEGIN svcGetProcessInfo
	str r0, [sp, #-0x4]!
	svc 0x2B
	ldr r3, [sp], #4
	str r1, [r3]
	str r2, [r3,#4]
	bx  lr
SVC_END

SVC_BEGIN svcGetThreadInfo
	str r0, [sp, #-0x4]!
	svc 0x2C
	ldr r3, [sp], #4
	str r1, [r3]
	str r2, [r3,#4]
	bx  lr
SVC_END

SVC_BEGIN svcConnectToPort
	str r0, [sp, #-0x4]!
	svc 0x2D
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcSendSyncRequest
	svc 0x32
	bx  lr
SVC_END

SVC_BEGIN svcOpenProcess
	push {r0}
	svc 0x33
	pop {r2}
	str r1, [r2]
	bx  lr
SVC_END

SVC_BEGIN svcOpenThread
	push {r0}
	svc 0x34
	pop {r2}
	str r1, [r2]
	bx  lr
SVC_END

SVC_BEGIN svcGetProcessId
	str r0, [sp, #-0x4]!
	svc 0x35
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcGetProcessIdOfThread
	str r0, [sp, #-0x4]!
	svc 0x36
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcGetThreadId
	str r0, [sp, #-0x4]!
	svc 0x37
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcGetResourceLimit
	str r0, [sp, #-0x4]!
	svc 0x38
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcGetResourceLimitLimitValues
	svc 0x39
	bx  lr
SVC_END

SVC_BEGIN svcGetResourceLimitCurrentValues
	svc 0x3A
	bx  lr
SVC_END

SVC_BEGIN svcBreak
	svc 0x3C
	bx  lr
SVC_END

SVC_BEGIN svcOutputDebugString
	svc 0x3D
	bx  lr
SVC_END

SVC_BEGIN svcControlPerformanceCounter
	push  {r0}
	ldr   r0, [sp, #4+0]
	ldr   r3, [sp, #4+4]
	svc   0x3E
	pop   {r3}
	stmia r3, {r1, r2}
	bx    lr
SVC_END

SVC_BEGIN svcCreatePort
	push {r0, r1}
	svc 0x47
	ldr r3, [sp, #0]
	str r1, [r3]
	ldr r3, [sp, #4]
	str r2, [r3]
	add sp, sp, #8
	bx  lr
SVC_END

SVC_BEGIN svcCreateSessionToPort
	push {r0}
	svc 0x48
	pop {r2}
	str r1, [r2]
	bx lr
SVC_END

SVC_BEGIN svcCreateSession
	push {r0, r1}
	svc 0x49
	ldr r3, [sp, #0]
	str r1, [r3]
	ldr r3, [sp, #4]
	str r2, [r3]
	add sp, sp, #8
	bx  lr
SVC_END

SVC_BEGIN svcAcceptSession
	str r0, [sp, #-4]!
	svc 0x4A
	ldr r2, [sp]
	str r1, [r2]
	add sp, sp, #4
	bx  lr
SVC_END

SVC_BEGIN svcReplyAndReceive
	str r0, [sp, #-4]!
	svc 0x4F
	ldr r2, [sp]
	str r1, [r2]
	add sp, sp, #4
	bx  lr
SVC_END

SVC_BEGIN svcBindInterrupt
	svc 0x50
	bx lr
SVC_END

SVC_BEGIN svcUnbindInterrupt
	svc 0x51
	bx lr
SVC_END

SVC_BEGIN svcInvalidateProcessDataCache
	svc 0x52
	bx  lr
SVC_END

SVC_BEGIN svcStoreProcessDataCache
	svc 0x53
	bx  lr
SVC_END

SVC_BEGIN svcFlushProcessDataCache
	svc 0x54
	bx  lr
SVC_END

SVC_BEGIN svcStartInterProcessDma
	stmfd sp!, {r0, r4, r5}
	ldr r0, [sp, #0xC]
	ldr r4, [sp, #0x10]
	ldr r5, [sp, #0x14]
	svc 0x55
	ldmfd sp!, {r2, r4, r5}
	str r1, [r2]
	bx  lr
SVC_END

SVC_BEGIN svcStopDma
	svc 0x56
	bx  lr
SVC_END

SVC_BEGIN svcGetDmaState
	str r0, [sp, #-4]!
	svc 0x57
	ldr r3, [sp], #4
	strb r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcRestartDma
	push {r4}
	ldr  r4, [sp, #4]
	svc  0x58
	pop  {r4}
	bx   lr
SVC_END

SVC_BEGIN svcSetGpuProt
	svc 0x59
	bx  lr
SVC_END

SVC_BEGIN svcSetWifiEnabled
	svc 0x5A
	bx  lr
SVC_END

SVC_BEGIN svcDebugActiveProcess
	push {r0}
	svc 0x60
	pop {r2}
	str r1, [r2]
	bx  lr
SVC_END

SVC_BEGIN svcBreakDebugProcess
	svc 0x61
	bx  lr
SVC_END

SVC_BEGIN svcTerminateDebugProcess
	svc 0x62
	bx  lr
SVC_END

SVC_BEGIN svcGetProcessDebugEvent
	svc 0x63
	bx  lr
SVC_END

SVC_BEGIN svcContinueDebugEvent
	svc 0x64
	bx  lr
SVC_END

SVC_BEGIN svcGetProcessList
	str r0, [sp, #-0x4]!
	svc 0x65
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcGetThreadList
	str r0, [sp, #-0x4]!
	svc 0x66
	ldr r3, [sp], #4
	str r1, [r3]
	bx  lr
SVC_END

SVC_BEGIN svcGetDebugThreadContext
	svc 0x67
	bx lr
SVC_END

SVC_BEGIN svcSetDebugThreadContext
	svc 0x68
	bx lr
SVC_END

SVC_BEGIN svcQueryDebugProcessMemory
	push {r0, r1, r4-r6}
	svc 0x69
	ldr r6, [sp]
	stm r6, {r1-r4}
	ldr r6, [sp, #4]
	str r5, [r6]
	add sp, sp, #8
	pop {r4-r6}
	bx  lr
SVC_END

SVC_BEGIN svcReadProcessMemory
	svc 0x6A
	bx  lr
SVC_END

SVC_BEGIN svcWriteProcessMemory
	svc 0x6B
	bx  lr
SVC_END

SVC_BEGIN svcSetHardwareBreakPoint
	svc 0x6C
	bx  lr
SVC_END

SVC_BEGIN svcGetDebugThreadParam
	push {r0, r1, r4, r5}
	ldr r0, [sp, #16]
	svc 0x6D
	pop {r4, r5}
	stm r4, {r1, r2}
	str r3, [r5]
	pop {r4, r5}
	bx lr
SVC_END

SVC_BEGIN svcControlProcessMemory
	push {r4-r5}
	ldr r4, [sp, #0x8]
	ldr r5, [sp, #0xC]
	svc 0x70
	pop {r4-r5}
	bx  lr
SVC_END

SVC_BEGIN svcMapProcessMemory
	svc 0x71
	bx  lr
SVC_END

SVC_BEGIN svcUnmapProcessMemory
	svc 0x72
	bx  lr
SVC_END

SVC_BEGIN svcCreateCodeSet
	str r0, [sp, #-0x4]!
	ldr r0, [sp, #4]
	svc 0x73
	ldr r2, [sp, #0]
	str r1, [r2]
	add sp, sp, #4
	bx  lr
SVC_END

SVC_BEGIN svcCreateProcess
	str r0, [sp, #-0x4]!
	svc 0x75
	ldr r2, [sp, #0]
	str r1, [r2]
	add sp, sp, #4
	bx  lr
SVC_END

SVC_BEGIN svcTerminateProcess
	svc 0x76
	bx  lr
SVC_END

SVC_BEGIN svcSetProcessResourceLimits
	svc 0x77
	bx  lr
SVC_END

SVC_BEGIN svcCreateResourceLimit
	push {r0}
	svc  0x78
	pop  {r2}
	str  r1, [r2]
	bx   lr
SVC_END

SVC_BEGIN svcSetResourceLimitValues
	svc 0x79
	bx  lr
SVC_END

SVC_BEGIN svcBackdoor
	svc 0x7B
	bx  lr
SVC_END

SVC_BEGIN svcKernelSetState
	svc 0x7C
	bx  lr
SVC_END

SVC_BEGIN svcQueryProcessMemory
	push {r0, r1, r4-r6}
	svc 0x7D
	ldr r6, [sp]
	stm r6, {r1-r4}
	ldr r6, [sp, #4]
	str r5, [r6]
	add sp, sp, #8
	pop {r4-r6}
	bx  lr
SVC_END
