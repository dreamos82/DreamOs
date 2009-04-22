#ifndef _DEBUG_H
#define _DEBUG_H

#include <io.h>

/******************************
 *		Debug Macros
 ******************************/
 
/**
 *  Used to enable the device. 
 *  Any I/O to the debug module before this command 
 *  is sent will simply be ignored. */
#define DBG_ENABLE		0x8A00

/**
 *  Disable the I/O interface to the debugger 
 *  and the memory monitoring functions. */
#define DBG_DISABLE		0x8AFF

/**
 *  Selects register 0: 
 *  Memory monitoring range start address (inclusive) */
#define SELECTS_REG_0	0x8A01

/**
 *  Selects register 1: 
 *  Memory monitoring range end address (exclusive) */
#define SELECTS_REG_1	0x8A02
 
/**
 *  Enable address range memory monitoring 
 *  as indicated by register 0 and 1 and 
 *  clears both registers */
#define ENABLE_ADDR_RANGE_MEM_MONITOR	0x8A80

/**
 *  If the debugger is enabled (via --enable-debugger), 
 *  sending 0x8AE0 to port 0x8A00 after the device 
 *  has been enabled will return the Bochs to the debugger prompt.
 *  Basically the same as doing CTRL+C. */
#define RET_TO_DBGPROMPT	0x8AE0

/**
 *  If the debugger is enabled (via --enable-debugger), 
 *  sending 0x8AE2 to port 0x8A00 after the device 
 *  has been enabled will enable instruction tracing */
#define INSTRUCTION_TRACE_ENABLE	0x8AE3

/**
 *  If the debugger is enabled (via --enable-debugger), 
 *  sending 0x8AE2 to port 0x8A00 after the device 
 *  has been enabled will disable instruction tracing */
#define INSTRUCTION_TRACE_DISABLE	0x8AE2

/**
 *  If the debugger is enabled (via --enable-debugger), 
 *  sending 0x8AE5 to port 0x8A00 after the device 
 *  has been enabled will enable register tracing. 
 *  This currently output the value of all the registers 
 *  for each instruction traced.
 *  Note: instruction tracing must be enabled to view 
 *  the register tracing */
#define REGISTER_TRACE_ENABLE	0x8AE5

/**
 *  If the debugger is enabled (via --enable-debugger), 
 *  sending 0x8AE4 to port 0x8A00 after the device 
 *  has been enabled will disable register tracing. */
#define REGISTER_TRACE_DISABLE	0x8AE4

void dbg_bochs_print( const unsigned char * );
inline void dbg_bochs_send_cmd( const int port, const int cmd );
#endif /* ! _DEBUG_H */
