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
#ifndef __BITMAPS_H__
#define __BITMAPS_H__

#include "main.h"

extern const uint8_t dragon_bitmap[];
extern const uint8_t dragon_bitmap_width;
extern const uint8_t dragon_bitmap_height;
extern const uint8_t wizard_bitmap[];
extern const uint8_t wizard_width;
extern const uint8_t wizard_height;
extern const uint8_t blue_orb_bitmap[];
extern const uint8_t blue_orb_width;
extern const uint8_t blue_orb_height;
extern const uint8_t shield_bitmap[];
extern const uint8_t shield_width;
extern const uint8_t shield_height;
extern const uint8_t menu_dragon[];
extern const uint8_t menu_dragon_width;
extern const uint8_t menu_dragon_height;
extern const uint8_t menu_title_bitmap[];
extern const uint8_t menu_title_width;
extern const uint8_t menu_title_height;
extern const uint8_t start_bitmap[];
extern const uint8_t start_width;
extern const uint8_t start_height;
extern const uint8_t high_score_bitmap[];
extern const uint8_t high_score_width;
extern const uint8_t high_score_height;
extern const uint8_t back_menu_bitmap[];
extern const uint8_t back_menu_width;
extern const uint8_t back_menu_height;
extern const uint8_t small_dragon_bitmap[];
extern const uint8_t small_dragon_width;
extern const uint8_t small_dragon_height;
extern const uint8_t fireball_bitmap[];
extern const uint8_t fireball_width;
extern const uint8_t fireball_height;
extern const uint8_t game_over_bitmap[];
extern const uint8_t game_over_width;
extern const uint8_t game_over_height;
extern const uint8_t you_win_bitmap[];
extern const uint8_t you_win_width;
extern const uint8_t you_win_height;

// This code below is skeleton FONT_INFO struct code from dot factory website
// This structure describes a single character's display information
typedef struct
{
	const uint8_t widthBits;					// width, in bits (or pixels), of the character
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
	
} FONT_CHAR_INFO;	

// Describes a single font
typedef struct
{
	const uint8_t 			heightPages;	// height, in pages (8 pixels), of the font's characters
	const uint8_t 			startChar;		// the first character in the font (e.g. in charInfo and data)
	const uint8_t 			endChar;		// the last character in the font
	const uint8_t			spacePixels;	// number of pixels that a space character takes up
	const FONT_CHAR_INFO*	charInfo;		// pointer to array of char information
	const uint8_t*			data;			// pointer to generated array of character visual representation
		
} FONT_INFO;

typedef struct
{
	uint8_t start_x_loc;
	uint8_t start_y_loc;
	uint8_t x_loc;
	uint8_t y_loc;
	bool isHit;
	
} Dragon_obj;

extern const uint8_t game_title_bitmap[];
extern const FONT_INFO game_title_font_info;
extern const FONT_CHAR_INFO game_title_descriptors[];

#endif