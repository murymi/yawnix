#pragma once
#include <stdint.h>
// The Data Port (IO Port 0x60) is used for 
// reading data that was received from a PS/2 device or from the PS/2
//controller itself and writing data to a PS/2 
// device or to the PS/2 controller itself.
#define PS2_DATA_PORT 0x60

// The Status Register contains various flags that show the state of the PS/2 controller
#define PS2_STATUS_PORT 0x60

#define PS2_COMMAND_PORT 0x64

typedef struct {
    /// Output buffer status (0 = empty, 1 = full)
    /// (must be set before attempting to read data from IO port 0x60) 
    uint32_t output_full:1;

    /// Input buffer status (0 = empty, 1 = full)
    /// (must be clear before attempting to write data to IO port 0x60 or IO port 0x64) 
    uint32_t input_full:1;
    /// System Flag
    /// Meant to be cleared on reset and set by firmware (via. PS/2 Controller Configuration Byte) if the system passes self tests (POST) 
    uint32_t system_flag:1;

 	/// Command/data (0 = data written to input buffer is data for PS/2 device, 1 = data written to input buffer is data for PS/2 controller command)     
    uint32_t command:1;
    /// Unknown (chipset specific)
    /// May be "keyboard lock" (more likely unused on modern systems) 
    uint32_t unknown: 1;
    /// Unknown (chipset specific)
    /// May be "receive time-out" or "second PS/2 port output buffer full" 
    uint32_t unknown2: 1;

    uint32_t time_out: 1;
    uint32_t parity_error: 1;
} 
__attribute__((packed))
ps2_status_t;