// Copyright (c) 2015-16, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "TM4C123.h"
#include "gpio_port.h"
#include "lcd.h"
#include "lcd_images.h"
#include "timers.h"
#include "ps2.h"
#include "launchpad_io.h"
#include "ft6x06.h"
#include "accel.h"
#include "eeprom.h"
#include "validate.h"
#include "io_expander.h"

#include "project_interrupts.h"
#include "project_hardware_init.h"
#include "project_bitmaps.h"

#define addrD               80
#define addrK               160
#define addrHighscore       240

#define SCREEN_X            240
#define SCREEN_Y            320

#define SCREEN_CENTER_X    ((SCREEN_X/2)-1)
#define SCREEN_CENTER_Y    ((SCREEN_Y/2)-1)

#define LINE_WIDTH          11
#define LINE_LENGTH         (SCREEN_X - 1)

#define SQUARE_SIZE         64

#define PADDING             4

#define LEFT_X                      (SCREEN_CENTER_X - SQUARE_SIZE - (2*PADDING) - LINE_WIDTH)
#define CENTER_X                    (SCREEN_CENTER_X)
#define RIGHT_X                     (SCREEN_CENTER_X + SQUARE_SIZE + (2*PADDING) + LINE_WIDTH)

#define UPPER_Y                     (SCREEN_CENTER_Y - SQUARE_SIZE - (2*PADDING) - LINE_WIDTH)
#define CENTER_Y                    (SCREEN_CENTER_Y)
#define LOWER_Y                     (SCREEN_CENTER_Y + SQUARE_SIZE + (2*PADDING) + LINE_WIDTH)

#define UPPER_HORIZONTAL_LINE_Y     (SCREEN_CENTER_Y - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2)
#define LOWER_HORIZONTAL_LINE_Y     (SCREEN_CENTER_Y + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2)

#define LEFT_VERTICAL_LINE_X      (SCREEN_CENTER_X - (SQUARE_SIZE/2) - PADDING - LINE_WIDTH/2)
#define RIGHT_VERTICAL_LINE_X     (SCREEN_CENTER_X + (SQUARE_SIZE/2) + PADDING + LINE_WIDTH/2)

#endif
