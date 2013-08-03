#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <wiringPi.h>

/*
 * Definitions
*/
#define DS		0
#define SHCP	2
#define STCP	3
#define CYCLE	700
#define DELAY	10

/*
 * Arrays for digits
 * Since this is a common annode
 * 7 segment display, 0 = LED on
 * and 1 = LED off
 *
 * template
 * {dp, a, b, c, d, e, f, g};
*/
uint8_t my_index[10][8] = {
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 1, 1, 1, 1},
	{1, 0, 0, 1, 0, 0, 1, 0},
	{1, 0, 0, 0, 0, 1, 1, 0},
	{1, 1, 0, 0, 1, 1, 0, 0},
	{1, 0, 1, 0, 0, 1, 0, 0},
	{1, 0, 1, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 1, 1, 0, 0}};

/*
 * Forward declarations
*/
void shift_digit(uint8_t digit);
void load_digits(struct tm *cur_time);

/*
 * Main
*/
int main(int argc, char *argv[])
{
	time_t sys_time;
	struct tm *my_time;

	if(wiringPiSetup() == -1) {
		// printf("wiringpi not set up\n");
		return 1;
	}
	
	pinMode(DS, OUTPUT);
	pinMode(STCP, OUTPUT);
	pinMode(SHCP, OUTPUT);

	for(;;) {
		sys_time = time(NULL);
		my_time = localtime(&sys_time);
		load_digits(my_time);
		delay(CYCLE);
	}

	return 0;
}

/*
 * Functions
*/
void shift_digit(uint8_t digit)
{
	int i = 7;
	// make sure SHCP, STCP start low
	digitalWrite(SHCP, 0);
	digitalWrite(STCP, 0);

	for(i = 7; i >= 0; i--){
		digitalWrite(DS, my_index[digit][i]);
		delay(DELAY);
		digitalWrite(SHCP, 1);
		delay(DELAY);
		digitalWrite(SHCP, 0);
		delay(DELAY);
		// printf("%d ", my_index[digit][i]);
	}
}

void load_digits(struct tm *cur_time)
{
	// make sure STCP is low
	digitalWrite(STCP, 0);

	// get time
	uint8_t min_tens = cur_time->tm_min / 10;
	uint8_t min_ones = cur_time->tm_min % 10;
	uint8_t hour_tens = cur_time->tm_hour / 10;
	uint8_t hour_ones = cur_time->tm_hour % 10;
	// printf("Time: %d%d:%d%d\n",
	//		hour_tens, hour_ones, min_tens, min_ones);

	// shift out
	// printf("m: ");
	shift_digit(min_ones);
	// printf("\nM: ");
	shift_digit(min_tens);
	// printf("\nh: ");
	shift_digit(hour_ones);
	// printf("\nH: ");
	shift_digit(hour_tens);
	// printf("\n");

	// load into storage
	digitalWrite(STCP, 1);
	delay(DELAY);
	digitalWrite(STCP, 0);
}
