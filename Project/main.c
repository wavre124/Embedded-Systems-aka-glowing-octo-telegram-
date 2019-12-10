// Copyright (c) 2015-19, Joe Krachey
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

#include "main.h"

Dragon_obj dragons[5];

//*****************************************************************************
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

static void lcd_display_menu_options(void)
{
	lcd_draw_rectangle(CENTER_X, 180, LOWER_Y - 30, start_height + 10, LCD_COLOR_GRAY);
	lcd_draw_image(CENTER_X, start_width, LOWER_Y - 30, start_height, start_bitmap, LCD_COLOR_ORANGE, LCD_COLOR_GRAY);
	lcd_draw_rectangle(CENTER_X, 180, LOWER_Y + 30, high_score_height + 10, LCD_COLOR_GRAY);
	lcd_draw_image(CENTER_X, high_score_width, LOWER_Y + 30, high_score_height, high_score_bitmap, LCD_COLOR_ORANGE, LCD_COLOR_GRAY);
}

static void lcd_display_menu_text(void) 
{
	lcd_draw_image(CENTER_X, menu_title_width, CENTER_Y, menu_title_height, menu_title_bitmap, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
}

static void lcd_init_menu(void)
{
	lcd_clear_screen(LCD_COLOR_BLACK);
	
	lcd_draw_image(CENTER_X, menu_dragon_width, UPPER_Y, menu_dragon_height, menu_dragon, LCD_COLOR_RED, LCD_COLOR_BLACK);
	
	lcd_display_menu_text();
	lcd_display_menu_options();
}

static void lcd_boss_battle(int boss_x, int boss_y)
{
	
	lcd_draw_image(boss_x, menu_dragon_width, boss_y, menu_dragon_height, menu_dragon, LCD_COLOR_RED, LCD_COLOR_WHITE);
	
}

static void lcd_init_level(void)
{
	int i;
	
	lcd_clear_screen(LCD_COLOR_WHITE);
		
	for(i = 0; i < 5; i++) {
		lcd_draw_image(dragons[i].start_x_loc, small_dragon_width, dragons[i].start_y_loc, small_dragon_height, small_dragon_bitmap, LCD_COLOR_RED, LCD_COLOR_WHITE);
	}
	
	lcd_draw_image(CENTER_X, wizard_width, LOWER_Y, wizard_height, wizard_bitmap, LCD_COLOR_BROWN, LCD_COLOR_WHITE);
}

static void lcd_redraw_field(int knight_x, int knight_y, int old_x, int old_y, bool shield)
{
	if(shield) {
		lcd_clear_image_move(old_x, wizard_width + shield_width + 5, old_y, wizard_height + 5 + shield_height, LCD_COLOR_WHITE);
		lcd_draw_image(knight_x, wizard_width, knight_y, wizard_height, wizard_bitmap, LCD_COLOR_BROWN, LCD_COLOR_WHITE);
		lcd_draw_image(knight_x, shield_width, knight_y - 25, shield_height, shield_bitmap, LCD_COLOR_BLUE2, LCD_COLOR_WHITE);
	} else {
		lcd_clear_image_move(old_x, wizard_width, old_y, wizard_height + 5, LCD_COLOR_WHITE);
		lcd_draw_image(knight_x, wizard_width, knight_y, wizard_height, wizard_bitmap, LCD_COLOR_BROWN, LCD_COLOR_WHITE);
	}
	
}

static void lcd_high_score(void)
{
	lcd_clear_screen(LCD_COLOR_BLACK);
	// draw high score title
	lcd_draw_image(CENTER_X, high_score_width, UPPER_Y, high_score_height, high_score_bitmap, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
	// print out actual high score from EEPROM
	
	// print out back to menu button
	lcd_draw_rectangle(CENTER_X, 180, LOWER_Y, back_menu_height + 10, LCD_COLOR_GRAY);
	lcd_draw_image(CENTER_X, back_menu_width, LOWER_Y, back_menu_height, back_menu_bitmap, LCD_COLOR_ORANGE, LCD_COLOR_GRAY);
	
}

static void game_over_display(void)
{
	lcd_clear_screen(LCD_COLOR_BLACK);
	// derpy dragon
	lcd_draw_image(CENTER_X, dragon_bitmap_width, UPPER_Y, dragon_bitmap_height, dragon_bitmap, LCD_COLOR_RED, LCD_COLOR_BLACK);
	// game over
	lcd_draw_image(CENTER_X, game_over_width, CENTER_Y, game_over_height, game_over_bitmap, LCD_COLOR_ORANGE, LCD_COLOR_BLACK);
	// display score
}

static void wizard_teleport(int knight_x, int knight_y, int old_x, int old_y, bool shield)
{
	if(shield) {
		lcd_draw_rectangle(old_x, wizard_width + shield_width + 5, old_y, wizard_height + 5 + shield_height, LCD_COLOR_WHITE);
		lcd_draw_image(knight_x, wizard_width, knight_y, wizard_height, wizard_bitmap, LCD_COLOR_BROWN, LCD_COLOR_WHITE);
		lcd_draw_image(knight_x, shield_width, knight_y - 25, shield_height, shield_bitmap, LCD_COLOR_BLUE2, LCD_COLOR_WHITE);
	} else {
		lcd_draw_rectangle(old_x, wizard_width, old_y, wizard_height + 5, LCD_COLOR_WHITE);
		lcd_draw_image(knight_x, wizard_width, knight_y, wizard_height, wizard_bitmap, LCD_COLOR_BROWN, LCD_COLOR_WHITE);
	}
}

static void red_dragon_dissapear(int drago_x, int drago_y)
{
	lcd_draw_rectangle(drago_x, dragon_bitmap_width, drago_y, dragon_bitmap_height, LCD_COLOR_WHITE);
}

static void shield_triggered(int knight_x, int knight_y)
{
	lcd_draw_image(knight_x, shield_width, knight_y - 25, shield_height, shield_bitmap, LCD_COLOR_BLUE2, LCD_COLOR_WHITE);
}

static void shield_lowered(int knight_x, int knight_y)
{
	lcd_draw_rectangle(knight_x, shield_width + 5, knight_y - 25, shield_height, LCD_COLOR_WHITE);
}

static void wizard_spell(int shot_x, int shot_y)
{
	lcd_draw_image(shot_x, blue_orb_width, shot_y, blue_orb_height, blue_orb_bitmap, LCD_COLOR_BLUE2, LCD_COLOR_WHITE);
}

static void draw_fire_ball(int ball_x, int ball_y)
{
	lcd_draw_image(ball_x, fireball_width, ball_y, fireball_height, fireball_bitmap, LCD_COLOR_ORANGE, LCD_COLOR_WHITE);
}

static void shot_clear(int shot_x, int shot_y)
{
	lcd_draw_rectangle(shot_x, blue_orb_width, shot_y, blue_orb_height, LCD_COLOR_WHITE);
}

static void fire_ball_clear(int ball_x, int ball_y)
{
	lcd_draw_rectangle(ball_x, fireball_width, ball_y, fireball_height + 10, LCD_COLOR_WHITE);
}

static void delay(void)
{
	int i;
	for(i = 0; i < 5000; i++);
}

static void init_dragons(void)
{
	int i;
	for(i = 0; i < 5; i++) {
		dragons[i].isHit = false;
		
		// hard code dragon location values
		switch(i) {
			case 0:
				dragons[i].start_x_loc = CENTER_X;
				dragons[i].start_y_loc = UPPER_Y + 5;
				dragons[i].x_loc = CENTER_X;
				dragons[i].y_loc = UPPER_Y + 5;
				break;
		  case 1:
				dragons[i].start_x_loc = LEFT_X;
				dragons[i].start_y_loc = UPPER_Y + 5;
				dragons[i].x_loc = LEFT_X;
				dragons[i].y_loc = UPPER_Y + 5;
				break;
			case 2:
				dragons[i].start_x_loc = RIGHT_X;
				dragons[i].start_y_loc = UPPER_Y + 5;
				dragons[i].x_loc = RIGHT_X;
				dragons[i].y_loc = UPPER_Y + 5;
				break;
			case 3:
				dragons[i].start_x_loc = CENTER_X - 40;
				dragons[i].start_y_loc = UPPER_Y - 35;
				dragons[i].x_loc = CENTER_X - 40;
				dragons[i].y_loc = UPPER_Y - 35;
				break;
			case 4:
				dragons[i].start_x_loc = CENTER_X + 40;
				dragons[i].start_y_loc = UPPER_Y - 35;
				dragons[i].x_loc = CENTER_X + 40;
				dragons[i].y_loc = UPPER_Y - 35;
				break;
			default:
				dragons[i].start_x_loc = CENTER_X;
				dragons[i].start_y_loc = CENTER_Y;
				dragons[i].x_loc = CENTER_X;
				dragons[i].y_loc = CENTER_Y;
				break;
		}
		
	}
}

static void repaint_dragons(void)
{
	
	int i;
	int dead_dragons = 0;
	
	for(i = 0; i < 5; i++) {
		
		if(dragons[i].isHit) {
			dead_dragons += 1;
		}
	}
	
	for(i = 0; i < 5; i++) {
		
		if(dragons[i].isHit && (dead_dragons > 3)) {
			delay();
			lcd_draw_image(dragons[i].start_x_loc, small_dragon_width, dragons[i].start_y_loc, small_dragon_height, small_dragon_bitmap, LCD_COLOR_RED, LCD_COLOR_WHITE);
			dragons[i].isHit = false;
		}
		
	}
	
}

static void clear_dragons(void)
{
	int i;
	int dead_dragons = 0;
	
	for(i = 0; i < 5; i++) {
		red_dragon_dissapear(dragons[i].x_loc, dragons[i].y_loc);
	}
	
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	
	uint16_t t_x,t_y;
  uint8_t touch_event;
	int16_t accel_x, accel_y;
	// this will be the user score
	uint32_t player_score = 0;
	// this will use EEPROM
	uint32_t high_score = 0;
	int i;
	int knight_x = CENTER_X;
	int knight_y = LOWER_Y;
	int old_k_x = 0;
	int old_k_y = 0;
	int red_dragon_x = 0;
	int red_dragon_y = 0;
	int shot_x = 0;
	int shot_y = 0;
	int fire_x = 0;
	int fire_y = 0;
	int collide_x = 0;
	int collide_y = 0;
	int knight_collide_x = 0;
	int knight_collide_y = 0;
	int dragon_tick = 0;
	bool isUpPressed = false;
	bool move = false;
	bool red_dead = false;
	bool knight_reached_edge = false;
	bool move_right = false;
	bool move_left = false;
	bool shield_raised = false;
	bool wizard_shot = false;
	bool fire_ball = false;
	bool in_menu = false;
	bool high_score_bool = false;
	bool boss_battle = false;
	char msg[80];
	
	init_dragons();
	
	// menu and high score logic
	
	initialize_serial_debug();
	initialize_hardware();
	
	lcd_init_menu();
	
	while(in_menu) {
		touch_event = ft6x06_read_td_status();
		
		if(touch_event) {
			t_x = ft6x06_read_x();
			t_y = ft6x06_read_y();
		} else {
			t_x = 500;
			t_y = 500;
		}
		
		if((t_x > 100) && (t_x < 120)){
			if((t_y > 180) && (t_y < 220)){
				in_menu = false;
			}
		} 
		
		if((t_x > 100) && (t_x < 120)){
			if((t_y > 260) && (t_y < 280)){
				high_score_bool = true;
			}
		} 
		
		if(high_score_bool) {
			lcd_high_score();
		}
		
		while(high_score_bool) {
			touch_event = ft6x06_read_td_status();
		
			if(touch_event) {
				t_x = ft6x06_read_x();
				t_y = ft6x06_read_y();
			} else {
				t_x = 500;
				t_y = 500;
			}
			
			if((t_x > 100) && (t_x < 120)){
				if((t_y > 220) && (t_y < 240)){
					high_score_bool = false;
					lcd_init_menu();
				}
			} 
			
		}
	}
	
	lcd_init_level();

    while(1)
		{
			io_expander_read_reg(MCP23017_GPIOB_R);
			
			old_k_x = knight_x;
			old_k_y = knight_y;
			
			touch_event = ft6x06_read_td_status();
			
			if(wizard_shot) {
				shot_y -= 1;
				wizard_spell(shot_x, shot_y);
				
				if(shot_y < 20) {
					shot_clear(shot_x, shot_y);
					wizard_shot = false;
				}
				
				if(!boss_battle) {
				// loop through array of dragons and see if hit
					for(i = 0; i < 5; i++) {
					
					
						red_dragon_x = dragons[i].start_x_loc;
						red_dragon_y = dragons[i].start_y_loc;
					
						
						collide_x = abs(shot_x - red_dragon_x);
						collide_y = abs(shot_y - red_dragon_y);
					
					
						if(((collide_x < 20) && (collide_y < 20)) && !dragons[i].isHit) {
							shot_clear(shot_x, shot_y);
							red_dragon_dissapear(red_dragon_x, red_dragon_y);
							wizard_shot = false;
							red_dead = true;
							red_dragon_x = 500;
							red_dragon_y = 500;
							// player score increments whenever this happens
							player_score += 1;
							dragons[i].isHit = true;
						}
					
					}
				} else {
					sprintf(msg, "IN BOSS BATTLE \n\r");
					put_string(msg);
				}
				
			}
			
			if(fire_ball) {
				fire_y += 1;
				draw_fire_ball(fire_x, fire_y);
				
				if(fire_y > 280) {
					fire_ball_clear(fire_x, fire_y);
					fire_ball = false;
				}
				
				knight_collide_x = abs(fire_x - knight_x);
				knight_collide_y = abs(fire_y - knight_y);
				
				if(((knight_collide_x < 20) && (knight_collide_y < 20)) && !shield_raised) {
					//fire_ball_clear(fire_x, fire_y);
					// display game over
					delay();
					game_over_display();
					fire_ball = false;
					break;
				}
				
				if(((knight_collide_x < 50) && (knight_collide_y < 50)) && shield_raised) {
					fire_ball_clear(fire_x, fire_y);
					fire_ball = false;
				} 
				
				
				
			}
			
			if(touch_event) {
				t_x = ft6x06_read_x();
				t_y = ft6x06_read_y();
			} else {
				t_x = 500;
				t_y = 500;
			}
			
			accel_x = accel_read_x();
			accel_y = accel_read_y();
			
			if(left) {
				
				if(knight_x > 20) {
					knight_x -= 1;
				}
				move = true;
				
			} else if(right) {
				
				if(knight_x < 220) {
					knight_x += 1;
				}
				move = true;
				
			} else {
				// center
				move = false;
			}
				
			if(move) {
				lcd_redraw_field(knight_x, knight_y, old_k_x, old_k_y, shield_raised);
			}
			
			if(accel_x > 10000) {
				move_left = true;
				move_right = false;
			} else if(accel_x < -10000) {
				move_left = false;
				move_right = true;
			} else {
				move_left = false;
				move_right = false;
			}
			
			if(accel_y > 10000) {
				shield_triggered(knight_x, knight_y);
				shield_raised = true;
			} else {
				shield_lowered(knight_x, knight_y);
				shield_raised = false;
			}
			
			if(move_left) {
				if(knight_x > 119) {
				  knight_x = 119;
				  wizard_teleport(knight_x, knight_y, old_k_x, old_k_y, shield_raised);
				} else {
					knight_x = 21;
				  wizard_teleport(knight_x, knight_y, old_k_x, old_k_y, shield_raised);
				}
			} else if(move_right) {
				if(knight_x < 119) {
					knight_x = 119;
				  wizard_teleport(knight_x, knight_y, old_k_x, old_k_y, shield_raised);
				} else {
					knight_x = 219;
				  wizard_teleport(knight_x, knight_y, old_k_x, old_k_y, shield_raised);
				}
			} else {
				// do nothing
			}
			
			if(U && !shield_raised){
				//sprintf(msg, "WIZARD SPELL\n\r");
				//put_string(msg);
				//U = false;
				// shot will be shot
				shot_x = knight_x;
				shot_y = knight_y - wizard_height - 9;
				wizard_spell(shot_x, shot_y);
				wizard_shot = true;
			}
			
			if(timer_tick && !boss_battle) {
				// one of the dragons will shoot a fireball
				//sprintf(msg, "FIREBALL\n\r");
				//put_string(msg);
				// is this dangerous!?
				timer_tick = false;
				
				// choose which dragon ends up shooting fireball
				fire_x = dragons[dragon_tick].x_loc;
				fire_y = dragons[dragon_tick].y_loc + 30;
				
				if(!dragons[dragon_tick].isHit) {
					draw_fire_ball(fire_x, fire_y);
					fire_ball = true;
				}
				
				dragon_tick++;
				if(dragon_tick == 5) {
					dragon_tick = 0;
				}
			}
			
			if(!boss_battle) {
				repaint_dragons();
			}
				
			//sprintf(msg, "SCORE: %d\n\r", player_score);
			//put_string(msg);
			
			if((player_score > 5) && !boss_battle) {
				boss_battle = true;
				// get LCD ready for boss
				clear_dragons();
				// draw boss
				lcd_boss_battle(CENTER_X, UPPER_Y);
			}
			
			if(boss_battle) {
				// controls LEDs 
				
				
			}
			
			if (U) {
				sprintf(msg, "UP PRESSED \n\r");
				put_string(msg);
				U = false;
				io_expander_read_reg(MCP23017_GPIOB_R);
			} else if (D) {
				D = false;
				io_expander_read_reg(MCP23017_GPIOB_R);
			} else if (L) {
				L = false;
				io_expander_read_reg(MCP23017_GPIOB_R);
			} else if (R) {
				R = false;
				io_expander_read_reg(MCP23017_GPIOB_R);
			}
			
		}
}
