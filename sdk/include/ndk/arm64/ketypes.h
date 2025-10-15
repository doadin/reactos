

#ifndef _ARM64_KETYPES_H
#define _ARM64_KETYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Interrupt request levels */
#define PASSIVE_LEVEL           0
#define LOW_LEVEL               0
#define APC_LEVEL               1
#define DISPATCH_LEVEL          2
#define CMCI_LEVEL              5
#define CLOCK_LEVEL             13
#define IPI_LEVEL               14
#define DRS_LEVEL               14
#define POWER_LEVEL             14
#define PROFILE_LEVEL           15
#define HIGH_LEVEL              15

//
// IPI Types
//
#define IPI_APC                 1
#define IPI_DPC                 2
#define IPI_FREEZE              4
#define IPI_PACKET_READY        6
#define IPI_SYNCH_REQUEST       16

//
// PRCB Flags
//
#define PRCB_MAJOR_VERSION      1
#define PRCB_BUILD_DEBUG        1
#define PRCB_BUILD_UNIPROCESSOR 2

//
// No LDTs on ARM64
//
#define LDT_ENTRY              ULONG


//
// HAL Variables
//
#define INITIAL_STALL_COUNT     100
#define MM_HAL_VA_START         0xFFFFFFFFFFC00000ULL
#define MM_HAL_VA_END           0xFFFFFFFFFFFFFFFFULL

//
// Double fault stack size
//
#define DOUBLE_FAULT_STACK_SIZE 0x8000

//
// Structure for CPUID info
//
typedef union _CPU_INFO
{
    ULONG dummy;
} CPU_INFO, *PCPU_INFO;

typedef struct _KTRAP_FRAME
{
    UCHAR ExceptionActive;
    UCHAR ContextFromKFramesUnwound;
    UCHAR DebugRegistersValid;
    union
    {
        struct
        {
            CHAR PreviousMode;
            UCHAR PreviousIrql;
        };
    };
    ULONG Reserved;
    union
    {
        struct
        {
            ULONG64 FaultAddress;
            ULONG64 TrapFrame;
        };
    };
    //struct PKARM64_VFP_STATE VfpState;
    ULONG VfpState;
    ULONG Bcr[8];
    ULONG64 Bvr[8];
    ULONG Wcr[2];
    ULONG64 Wvr[2];
    ULONG Spsr;
    ULONG Esr;
    ULONG64 Sp;
    union
    {
        ULONG64 X[19];
        struct
        {
            ULONG64 X0;
            ULONG64 X1;
            ULONG64 X2;
            ULONG64 X3;
            ULONG64 X4;
            ULONG64 X5;
            ULONG64 X6;
            ULONG64 X7;
            ULONG64 X8;
            ULONG64 X9;
            ULONG64 X10;
            ULONG64 X11;
            ULONG64 X12;
            ULONG64 X13;
            ULONG64 X14;
            ULONG64 X15;
            ULONG64 X16;
            ULONG64 X17;
            ULONG64 X18;
        };
    };
    ULONG64 Lr;
    ULONG64 Fp;
    ULONG64 Pc;
} KTRAP_FRAME, *PKTRAP_FRAME;

typedef struct _KEXCEPTION_FRAME
{
    ULONG dummy;
} KEXCEPTION_FRAME, *PKEXCEPTION_FRAME;

#ifndef NTOS_MODE_USER

typedef struct _TRAPFRAME_LOG_ENTRY
{
    ULONG64 Thread;
    UCHAR CpuNumber;
    UCHAR TrapType;
    USHORT Padding;
    ULONG Cpsrl;
    ULONG64 X0;
    ULONG64 X1;
    ULONG64 X2;
    ULONG64 X3;
    ULONG64 X4;
    ULONG64 X5;
    ULONG64 X6;
    ULONG64 X7;
    ULONG64 Fp;
    ULONG64 Lr;
    ULONG64 Sp;
    ULONG64 Pc;
    ULONG64 Far;
    ULONG Esr;
    ULONG Reserved1;
} TRAPFRAME_LOG_ENTRY, *PTRAPFRAME_LOG_ENTRY;

//
// Processor Region Control Block
// Based on WoA
//
typedef struct _KPRCB {
    // Common fields (XP baseline)
    USHORT MinorVersion;                  // 0x000
    USHORT MajorVersion;                  // 0x002
    struct _KTHREAD* CurrentThread;       // x86: 0x004 / x64: 0x008
    struct _KTHREAD* NextThread;          // x86: 0x008 / x64: 0x010
    struct _KTHREAD* IdleThread;          // x86: 0x00C / x64: 0x018
    UCHAR Number;                         // x86: 0x010 / x64: 0x020
    UCHAR Reserved0;                      // x86: 0x011 / x64: 0x021
    USHORT BuildType;                     // x86: 0x012 / x64: 0x022
    ULONG SetMember;                      // x86: 0x014 / x64: 0x024

#if defined(_WIN64)
    UCHAR Group;                          // 0x028
    UCHAR GroupIndex;                     // 0x029
    USHORT GroupSetMember;                // 0x02A
    ULONG64 PrcbLock;                     // 0x030
#if (NTDDI_VERSION >= NTDDI_WIN8)
    VOID* PriorityState;
#endif
    ULONG64 PrcbPad00[3];                 // 0x038
#else
    CHAR CpuType;                         // 0x018
    CHAR CpuID;                           // 0x019
    USHORT CpuStep;                       // 0x01A
    ULONG MHz;                            // 0x01C
    ULONG HalReserved[8];                 // 0x020
    UCHAR PrcbPad00[0x20];                // 0x040
#endif

    struct _KSPIN_LOCK_QUEUE LockQueue[17]; // x86: 0x060 / x64: 0x050
    struct _PROCESSOR_STATE ProcessorState; // x86: 0x130 / x64: 0x150

#if defined(_WIN64)
    ULONG64 KernelReserved[16];           // 0x3F0
    ULONG64 HalReserved[16];              // 0x470
    ULONG64 PrcbPad01[16];                // 0x4F0
#else
    ULONG KernelReserved[14];             // 0x1B0
    ULONG HalReserved2[16];               // 0x1E8
    struct _KPRCB* MultiThreadProcessorSet; // 0x228
    struct _KPRCB* MultiThreadSetMaster;    // 0x22C
    ULONG SecondaryColorMask;             // 0x230
    ULONG DpcTime;                        // 0x234
    ULONG DpcTimeCount;                   // 0x238
    ULONG DpcTimeLimit;                   // 0x23C
    ULONG PrcbPad10[0x10];                // 0x240
#endif

    struct _KDPC_DATA DpcData[2];         // x86: 0x280 / x64: 0x570
    VOID* DpcStack;                       // x86: 0x2A0 / x64: 0x5B0

#if defined(_WIN64)
    LONG MaximumDpcQueueDepth;            // 0x5B8
    ULONG DpcRequestRate;                 // 0x5BC
    ULONG MinimumDpcRate;                 // 0x5C0
    ULONG DpcLastCount;                   // 0x5C4
    ULONG64 TimerHand;                    // 0x5C8
    ULONG64 TimerRequest;                 // 0x5D0
    ULONG64 TickOffset;                   // 0x5D8
    ULONG64 MasterOffset;                 // 0x5E0
    ULONG64 InterruptCount;               // 0x5E8
    ULONG64 KernelTime;                   // 0x5F0
    ULONG64 UserTime;                     // 0x5F8
    ULONG64 DpcTime;                      // 0x600
    ULONG64 InterruptTime;                // 0x608
    ULONG64 AdjustDpcThreshold;           // 0x610
    ULONG64 PageColor;                    // 0x618
    ULONG64 DebugDpcTime;                 // 0x620
    ULONG64 PrcbPad20[16];                // 0x628
#else
    ULONG MaximumDpcQueueDepth;           // 0x2A4
    ULONG DpcRequestRate;                 // 0x2A8
    ULONG MinimumDpcRate;                 // 0x2AC
    ULONG DpcLastCount;                   // 0x2B0
    ULONG PrcbLock;                       // 0x2B4
#if (NTDDI_VERSION >= NTDDI_WIN8)
    VOID* PriorityState;
#endif
    ULONG DpcInterruptRequested;          // 0x2B8
    ULONG DpcThreadRequested;             // 0x2BC
    ULONG DpcRoutineActive;               // 0x2C0
    ULONG DpcThreadActive;                // 0x2C4
    ULONG TimerHand;                      // 0x2C8
    ULONG TimerRequest;                   // 0x2CC
    ULONG TickOffset;                     // 0x2D0
    ULONG MasterOffset;                   // 0x2D4
    ULONG InterruptCount;                 // 0x318
    ULONG KernelTime;                     // 0x31C
    ULONG UserTime;                       // 0x320
    ULONG DpcTime2;                       // 0x324
    ULONG InterruptTime;                  // 0x328
    ULONG AdjustDpcThreshold;             // 0x32C
    ULONG PageColor;                      // 0x330
    ULONG DebugDpcTime;                   // 0x334
    ULONG PrcbPad30[0x10];                // 0x338
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10)
    VOID* SchedulerAssist;
    VOID* HypervisorData;
    VOID* VsmData;
#endif

#if defined(_WIN64)
    VOID* IumData;
    BOOLEAN IumEnabled;
    UCHAR PrcbPad30[7];
    KAFFINITY CoreProcessorSet;
    KAFFINITY PackageProcessorSet;
    KAFFINITY GroupAffinityMask;
    ULONG64 PrcbPad40[16];
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10)
    VOID* Reserved[64]; // Future-proofing
#endif

} KPRCB, *PKPRCB;


//
// Processor Control Region
// Based on WoA
//
typedef struct _KIPCR
{
    union
    {
        struct
        {
            ULONG TibPad0[2];
            PVOID Spare1;
            struct _KPCR *Self;
            PVOID  PcrReserved0;
            struct _KSPIN_LOCK_QUEUE* LockArray;
            PVOID Used_Self;
        };
    };
    KIRQL CurrentIrql;
    UCHAR SecondLevelCacheAssociativity;
    UCHAR Pad1[2];
    USHORT MajorVersion;
    USHORT MinorVersion;
    ULONG StallScaleFactor;
    ULONG SecondLevelCacheSize;
    struct
    {
        UCHAR ApcInterrupt;
        UCHAR DispatchInterrupt;
    };
    USHORT InterruptPad;
    UCHAR BtiMitigation;
    struct
    {
        UCHAR SsbMitigationFirmware:1;
        UCHAR SsbMitigationDynamic:1;
        UCHAR SsbMitigationKernel:1;
        UCHAR SsbMitigationUser:1;
        UCHAR SsbMitigationReserved:4;
    };
    UCHAR Pad2[2];
    ULONG64 PanicStorage[6];
    PVOID KdVersionBlock;
    PVOID HalReserved[134];
    PVOID KvaUserModeTtbr1;

    /* Private members, not in ntddk.h */
    PVOID Idt[256];
    PVOID* IdtExt;
    PVOID PcrAlign[15];
    KPRCB Prcb;
} KIPCR, *PKIPCR;

//
// Special Registers Structure (outside of CONTEXT)
// Based on WoA symbols
//
typedef struct _KSPECIAL_REGISTERS
{
    ULONG64 Elr_El1;
    UINT32  Spsr_El1;
    ULONG64 Tpidr_El0;
    ULONG64 Tpidrro_El0;
    ULONG64 Tpidr_El1;
    ULONG64 KernelBvr[8];
    ULONG   KernelBcr[8];
    ULONG64 KernelWvr[2];
    ULONG   KernelWcr[2];
} KSPECIAL_REGISTERS, *PKSPECIAL_REGISTERS;

//
// ARM64 Architecture State
// Based on WoA symbols
//
typedef struct _KARM64_ARCH_STATE
{
    ULONG64 Midr_El1;
    ULONG64 Sctlr_El1;
    ULONG64 Actlr_El1;
    ULONG64 Cpacr_El1;
    ULONG64 Tcr_El1;
    ULONG64 Ttbr0_El1;
    ULONG64 Ttbr1_El1;
    ULONG64 Esr_El1;
    ULONG64 Far_El1;
    ULONG64 Pmcr_El0;
    ULONG64 Pmcntenset_El0;
    ULONG64 Pmccntr_El0;
    ULONG64 Pmxevcntr_El0[31];
    ULONG64 Pmxevtyper_El0[31];
    ULONG64 Pmovsclr_El0;
    ULONG64 Pmselr_El0;
    ULONG64 Pmuserenr_El0;
    ULONG64 Mair_El1;
    ULONG64 Vbar_El1;
} KARM64_ARCH_STATE, *PKARM64_ARCH_STATE;

typedef struct _KPROCESSOR_STATE
{
    KSPECIAL_REGISTERS SpecialRegisters; // 0
    KARM64_ARCH_STATE ArchState;         // 160
    CONTEXT ContextFrame;                // 800
} KPROCESSOR_STATE, *PKPROCESSOR_STATE;

//
// Macro to get current KPRCB
//
FORCEINLINE
struct _KPRCB *
KeGetCurrentPrcb(VOID)
{  
    //UNIMPLEMENTED;
    return 0;
}

//
// Just read it from the PCR
//
#define KeGetCurrentIrql()             KeGetPcr()->CurrentIrql
#define _KeGetCurrentThread()          KeGetCurrentPrcb()->CurrentThread
#define _KeGetPreviousMode()           KeGetCurrentPrcb()->CurrentThread->PreviousMode
#define _KeIsExecutingDpc()            (KeGetCurrentPrcb()->DpcRoutineActive != 0)
#define KeGetCurrentThread()           _KeGetCurrentThread()
#define KeGetPreviousMode()            _KeGetPreviousMode()

#endif // !NTOS_MODE_USER

#ifdef __cplusplus
}; // extern "C"
#endif

#endif // !_ARM64_KETYPES_H
