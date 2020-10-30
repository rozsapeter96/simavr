#undef F_CPU
#define F_CPU 16000000
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega128");

#define __AVR_ATmega128__ 1
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

// GENERAL INIT - USED BY ALMOST EVERYTHING ----------------------------------

static void port_init()
{
	PORTA = 0b00011111;
	DDRA = 0b01000000; // buttons & led
	PORTB = 0b00000000;
	DDRB = 0b00000000;
	PORTC = 0b00000000;
	DDRC = 0b11110111; // lcd
	PORTD = 0b11000000;
	DDRD = 0b00001000;
	PORTE = 0b00100000;
	DDRE = 0b00110000; // buzzer
	PORTF = 0b00000000;
	DDRF = 0b00000000;
	PORTG = 0b00000000;
	DDRG = 0b00000000;
}

// TIMER-BASED RANDOM NUMBER GENERATOR ---------------------------------------

static void rnd_init()
{
	TCCR0 |= (1 << CS00); // Timer 0 no prescaling (@FCPU)
	TCNT0 = 0;			  // init counter
}

// generate a value between 0 and max
static int rnd_gen(int max)
{
	return TCNT0 % max;
}

// BUTTON HANDLING -----------------------------------------------------------

#define BUTTON_NONE 0
#define BUTTON_CENTER 1
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3
#define BUTTON_UP 4
static int button_accept = 1;

static int button_pressed()
{
	// right
	if ((!(PINA & 0b00000001)) & button_accept)
	{					   // check state of button 1 and value of button_accept
		button_accept = 0; // button is pressed
		return BUTTON_RIGHT;
	}

	// up
	if ((!(PINA & 0b00000010)) & button_accept)
	{					   // check state of button 2 and value of button_accept
		button_accept = 0; // button is pressed
		return BUTTON_UP;
	}

	// center
	if ((!(PINA & 0b00000100)) & button_accept)
	{					   // check state of button 3 and value of button_accept
		button_accept = 0; // button is pressed
		return BUTTON_CENTER;
	}

	// left
	if ((!(PINA & 0b00010000)) & button_accept)
	{					   // check state of button 5 and value of button_accept
		button_accept = 0; // button is pressed
		return BUTTON_LEFT;
	}

	return BUTTON_NONE;
}

static void button_unlock()
{
	//check state of all buttons
	if (
		((PINA & 0b00000001) | (PINA & 0b00000010) | (PINA & 0b00000100) | (PINA & 0b00001000) | (PINA & 0b00010000)) == 31)
		button_accept = 1; //if all buttons are released button_accept gets value 1
}

// LCD HELPERS ---------------------------------------------------------------

#define CLR_DISP 0x00000001
#define DISP_ON 0x0000000C
#define DISP_OFF 0x00000008
#define CUR_HOME 0x00000002
#define CUR_OFF 0x0000000C
#define CUR_ON_UNDER 0x0000000E
#define CUR_ON_BLINK 0x0000000F
#define CUR_LEFT 0x00000010
#define CUR_RIGHT 0x00000014
#define CG_RAM_ADDR 0x00000040
#define DD_RAM_ADDR 0x00000080
#define DD_RAM_ADDR2 0x000000C0

//#define		ENTRY_INC	    0x00000007	//LCD increment
//#define		ENTRY_DEC	    0x00000005	//LCD decrement
//#define		SH_LCD_LEFT	  0x00000010	//LCD shift left
//#define		SH_LCD_RIGHT	0x00000014	//LCD shift right
//#define		MV_LCD_LEFT	  0x00000018	//LCD move left
//#define		MV_LCD_RIGHT	0x0000001C	//LCD move right

static void lcd_delay(unsigned int b)
{
	volatile unsigned int a = b;
	while (a)
		a--;
}

static void lcd_pulse()
{
	PORTC = PORTC | 0b00000100; //set E to high
	lcd_delay(1400);			//delay ~110ms
	PORTC = PORTC & 0b11111011; //set E to low
}

static void lcd_send(int command, unsigned char a)
{
	unsigned char data;

	data = 0b00001111 | a;				 //get high 4 bits
	PORTC = (PORTC | 0b11110000) & data; //set D4-D7
	if (command)
		PORTC = PORTC & 0b11111110; //set RS port to 0 -> display set to command mode
	else
		PORTC = PORTC | 0b00000001; //set RS port to 1 -> display set to data mode
	lcd_pulse();					//pulse to set D4-D7 bits

	data = a << 4;						 //get low 4 bits
	PORTC = (PORTC & 0b00001111) | data; //set D4-D7
	if (command)
		PORTC = PORTC & 0b11111110; //set RS port to 0 -> display set to command mode
	else
		PORTC = PORTC | 0b00000001; //set RS port to 1 -> display set to data mode
	lcd_pulse();					//pulse to set d4-d7 bits
}

static void lcd_send_command(unsigned char a)
{
	lcd_send(1, a);
}

static void lcd_send_data(unsigned char a)
{
	lcd_send(0, a);
}

static void lcd_init()
{
	//LCD initialization
	//step by step (from Gosho) - from DATASHEET

	PORTC = PORTC & 0b11111110;

	lcd_delay(10000);

	PORTC = 0b00110000; //set D4, D5 port to 1
	lcd_pulse();		//high->low to E port (pulse)
	lcd_delay(1000);

	PORTC = 0b00110000; //set D4, D5 port to 1
	lcd_pulse();		//high->low to E port (pulse)
	lcd_delay(1000);

	PORTC = 0b00110000; //set D4, D5 port to 1
	lcd_pulse();		//high->low to E port (pulse)
	lcd_delay(1000);

	PORTC = 0b00100000; //set D4 to 0, D5 port to 1
	lcd_pulse();		//high->low to E port (pulse)

	lcd_send_command(0x28);		// function set: 4 bits interface, 2 display lines, 5x8 font
	lcd_send_command(DISP_OFF); // display off, cursor off, blinking off
	lcd_send_command(CLR_DISP); // clear display
	lcd_send_command(0x06);		// entry mode set: cursor increments, display does not shift

	lcd_send_command(DISP_ON);	// Turn ON Display
	lcd_send_command(CLR_DISP); // Clear Display
}

static void lcd_send_text(char *str)
{
	while (*str)
		lcd_send_data(*str++);
}

static void lcd_send_line1(char *str)
{
	lcd_send_command(DD_RAM_ADDR);
	lcd_send_text(str);
}

static void lcd_send_line2(char *str)
{
	lcd_send_command(DD_RAM_ADDR2);
	lcd_send_text(str);
}

static char is_jumping = 0;
static int map_counter = 0;
static int counter = 0;
static int collision = 0;

#define PLAYFIELD_ROWS 16
static unsigned char playfield[PLAYFIELD_ROWS];
static unsigned char player_state;

// GRAPHICS ------------------------------------------------------------------

#define CHAR_AIR_OBSTACLE 0
#define CHAR_AIR_PLAYER 1
#define CHAR_LAND 2
#define CHAR_LAND_OBSTACLE 3
#define CHAR_LAND_PLAYER_ON_GROUND 4
#define CHAR_LAND_PLAYER_MIDAIR 5
#define CHAR_EMPTY 6
#define CHAR_UNUSED_1 7
#define CHAR_EMPTY_EMPTY 8
#define CHAR_ERROR 9

#define CHARMAP_SIZE 8
static unsigned char CHARMAP[CHARMAP_SIZE][8] = {
	{0, 0, 0, 0, 0, 0b10101, 0b01110, 0},									 // CHAR_AIR_OBSTACLE
	{0, 0, 0, 0, 0b00011, 0b11110, 0b00110, 0b00100},						 // CHAR_AIR_PLAYER
	{0, 0, 0, 0, 0, 0, 0, 0b11111},											 // CHAR_LAND
	{0, 0, 0, 0b00110, 0b01100, 0b00100, 0b00100, 0b11111},					 // CHAR_LAND_OBSTACLE
	{0, 0, 0, 0b00011, 0b11110, 0b00110, 0b00100, 0b11111},					 // CHAR_LAND_PLAYER_ON_GROUND
	{0b00011, 0b11110, 0b00110, 0b00100, 0, 0, 0, 0b11111},					 // CHAR_LAND_PLAYER_MIDAIR
	{0, 0, 0, 0, 0, 0, 0, 0},												 // CHAR_EMPTY
	{0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111} // CHAR_UNUSED_1
};

static unsigned char XLAT_CHAR[] = {
	CHAR_AIR_OBSTACLE,			// 0b0000
	CHAR_AIR_PLAYER,			// 0b0001
	CHAR_LAND,					// 0b0010
	CHAR_LAND_OBSTACLE,			// 0b0011
	CHAR_LAND_PLAYER_ON_GROUND, // 0b0100
	CHAR_LAND_PLAYER_MIDAIR,	// 0b0101
	CHAR_EMPTY,					// 0b0110
	CHAR_UNUSED_1,				// 0b0111
	CHAR_ERROR,					// 0b1000
	CHAR_ERROR,					// 0b1001
	CHAR_ERROR,					// 0b1010
	CHAR_ERROR,					// 0b1011
	CHAR_ERROR					// 0b1100
};

#define PLAYER_POS 1

static void chars_init()
{
	for (int c = 0; c < CHARMAP_SIZE; ++c)
	{
		lcd_send_command(CG_RAM_ADDR + c * 8);
		for (int r = 0; r < 8; ++r)
		{
			lcd_send_data(CHARMAP[c][r]);
		}
	}
}

static char get_ground(unsigned char c)
{
	return (c >> 3);
}

static char get_air(unsigned char c)
{
	return c & 0b111;
}

static char calculate(unsigned char ground, unsigned char air)
{
	return ground << 3 | air;
}

static char merge_player(char field)
{
	if (get_ground(player_state) != CHAR_LAND)
	{
		if (get_ground(field) == CHAR_LAND_OBSTACLE)
		{
			collision = 1;
		}
		else
		{
			field = calculate(get_ground(player_state), get_air(field));
		}
	}
	if (get_air(player_state) == CHAR_AIR_PLAYER)
	{
		if(get_air(field) == CHAR_AIR_OBSTACLE)
		{
			collision = 1;
		}
		else
		{
			field = calculate(get_ground(field), get_air(player_state));
		}
		
	}
	return field;
}

static void screen_update()
{
	lcd_send_command(DD_RAM_ADDR); //set to Line 1
	
	char merged_field = merge_player(playfield[PLAYER_POS]);

	for (int r1 = 0; r1 < PLAYFIELD_ROWS; ++r1)
	{
		int symbol = playfield[r1] & 0b111;
		if(r1 == PLAYER_POS)
		{
			symbol = get_air(merged_field);
		}
		lcd_send_data(XLAT_CHAR[symbol]);
	}

	lcd_send_command(DD_RAM_ADDR2); //set to Line 2

	for (int r2 = 0; r2 < PLAYFIELD_ROWS; ++r2)
	{
		int symbol = playfield[r2] >> 3;
		if(r2 == PLAYER_POS)
		{
			symbol = get_ground(merged_field);
		}
		lcd_send_data(XLAT_CHAR[symbol]);
	}
}

static void set_random_char(unsigned char *playfield)
{
	int val = rnd_gen(7);
	if (++map_counter % 4 == 0 && val % 2 == 0)
	{
		if (val % 3 == 0)
		{
			*playfield = CHAR_LAND_OBSTACLE << 3 | CHAR_EMPTY;
		}
		else
		{
			*playfield = CHAR_LAND << 3 | CHAR_AIR_OBSTACLE;
		}
	}
	else
	{
		*playfield = CHAR_LAND << 3 | CHAR_EMPTY;
	}
}

static void playfield_clear()
{
	for (int r = 0; r < PLAYFIELD_ROWS; ++r)
	{
		if (r > PLAYER_POS + 2)
		{
			set_random_char(&playfield[r]);
		}
		else
		{
			playfield[r] = CHAR_LAND << 3 | CHAR_EMPTY;
		}
	}
}

static void jump()
{
	switch (is_jumping)
	{
	case 1:
		player_state = CHAR_LAND_PLAYER_MIDAIR << 3 | CHAR_EMPTY;
		break;
	case 2:
		player_state = CHAR_LAND << 3 | CHAR_AIR_PLAYER;
		break;
	case 3:
		player_state = CHAR_LAND_PLAYER_MIDAIR << 3 | CHAR_EMPTY;
		break;
	case 4:
		player_state = CHAR_LAND_PLAYER_ON_GROUND << 3 | CHAR_EMPTY;
		is_jumping = 0;
		return;
	default:
		return;
	}
	++is_jumping;
}
static int step()
{

	memcpy(playfield, playfield + 1, PLAYFIELD_ROWS - 1);
	set_random_char(&playfield[PLAYFIELD_ROWS - 1]);
	return 0;
}

// THE GAME ==================================================================

int main()
{
	port_init();
	lcd_init();
	chars_init();
	rnd_init();

	// "Splash screen"
	lcd_send_line1("    Dino");
	lcd_send_line2("    by prozsa");

	// loop of the whole program, always restarts game
	while (1)
	{
		while (button_pressed() != BUTTON_CENTER)
		{
			button_unlock();
		}

		playfield_clear(); // set up new playfield
		counter = 0;
		is_jumping = 0;
		collision = 0;
		player_state = CHAR_LAND_PLAYER_ON_GROUND << 3 | CHAR_EMPTY;

		// loop of the game
		while (1)
		{
			++counter;
			volatile int button = button_pressed();

			if (button == BUTTON_RIGHT && !is_jumping)
			{
				is_jumping = 1;
			}

			jump();
			step();
			if (collision)
			{
				break;
			}

			button_unlock();
			screen_update();
		}

		lcd_send_line1("    GAME OVER   ");
		char score_line[17];
		sprintf(score_line, "Your score:%5d", counter);
		lcd_send_line2(score_line);

	} // end of program-loop, we never quit
}
