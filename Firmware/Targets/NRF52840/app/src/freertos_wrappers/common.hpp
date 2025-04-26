#pragma once

#include <stdint.h>

#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	uint32_t
#define portBASE_TYPE	long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL

	/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
	not need to be guarded with a critical section. */
	#define portTICK_TYPE_IS_ATOMIC 1
#endif

// using StackType_t = uint8_t;
// using TickType_t = uint32_t;
// using BaseType_t = int32_t*;
// using UBaseType_t = uint32_t;
// constexpr TickType_t portMAX_DELAY = UINT32_MAX;
#define PRIVILEGED_FUNCTION
#define configMINIMAL_STACK_SIZE                 ((uint16_t)64)

typedef struct xSTATIC_TCB
{
    int idk;
} StaticTask_t;