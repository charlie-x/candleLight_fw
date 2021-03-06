/*

The MIT License (MIT)

Copyright (c) 2016 Hubert Denkmair

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "dfu.h"
#include <stdint.h>
#include "stm32f0xx_hal.h"

#define SYSMEM_RESET_VECTOR            0x1fffC804
#define RESET_TO_BOOTLOADER_MAGIC_CODE 0xDEADBEEF
#define BOOTLOADER_STACK_POINTER       0x20002250

uint32_t dfu_reset_to_bootloader_magic;

void __initialize_hardware_early(void)
{
	if (dfu_reset_to_bootloader_magic == RESET_TO_BOOTLOADER_MAGIC_CODE) {
		void (*bootloader)(void) = (void (*)(void)) (*((uint32_t *) SYSMEM_RESET_VECTOR)); // system memory reset vector
		dfu_reset_to_bootloader_magic = 0;
		__set_MSP(BOOTLOADER_STACK_POINTER);
		bootloader();
		while (42);
	} else {
		SystemInit();
	}
}

void dfu_run_bootloader()
{
	dfu_reset_to_bootloader_magic = RESET_TO_BOOTLOADER_MAGIC_CODE;
	NVIC_SystemReset();
}
