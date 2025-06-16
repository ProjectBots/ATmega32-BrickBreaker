
#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "bit.h"
#include "joystick.h"
#include "utils/disp/display.h"
#include "utils/math.h"

#define PLAYER_LIFES_START 3  // Initial number of lifes the player has
#define LIFE_BAR_WIDTH 5

#define PLATFORM_SIZE 15	// width of the platform in pixels
#define BALL_SIZE 2			// width and height of the ball in pixels
#define BALL_VELOCITY 2.5f	// Speed in pixels per update

#define BLOCKS_ROWS 4
#define BLOCKS_COLUMNS 8
#define BLOCK_HEIGHT ((DISPLAY_HEIGHT - 2) / BLOCKS_ROWS)  // -2 for wall on the top and bottom
#define BLOCK_WIDTH (BLOCK_HEIGHT / 2)

// Because the display width may not be evenly divisible by the number of blocks,
// a gap might be present at the right edge of the display.
// This Gap will be walled off and does not count as part of the area where the player can play.
#define PLAYAREA_HEIGHT (BLOCK_HEIGHT * BLOCKS_ROWS)
#define PLAYAREA_WIDTH (DISPLAY_WIDTH - LIFE_BAR_WIDTH - 2)

// Game state variables
static bool gameWon = false;
static bool gameLost = false;

static uint8_t lifes = PLAYER_LIFES_START;
static uint8_t blockCount = BLOCKS_ROWS * BLOCKS_COLUMNS;

static float platformY = (PLAYAREA_HEIGHT - PLATFORM_SIZE) / 2.0f;	// Start in the middle of the play area
static float ballX;
static float ballY;
static float ballSpeedX;
static float ballSpeedY;

void initBall() {
	ballX = 1;
	ballY = platformY + PLATFORM_SIZE / 2.0f - BALL_SIZE / 2.0f;  // Start in the middle of the platform
	ballSpeedX = BALL_VELOCITY;
	ballSpeedY = 0;
}

static bool blocks[BLOCKS_ROWS][BLOCKS_COLUMNS];  // true means alive, false means hit

void initBlocks() {
	for (uint8_t row = 0; row < BLOCKS_ROWS; row++) {
		for (uint8_t col = 0; col < BLOCKS_COLUMNS; col++) {
			blocks[row][col] = true;  // All blocks are initially alive
		}
	}
}

void gameUpdate() {
	// parallelize joystick reading
	requestJoystickUpdate();

	// Update ball position
	ballX += ballSpeedX;
	ballY += ballSpeedY;

	// Check for wall collisions
	if (ballY < 0) {
		ballY = 0;
		ballSpeedY = -ballSpeedY;  // Bounce off top wall
	} else if (ballY > PLAYAREA_HEIGHT - BALL_SIZE) {
		ballY = PLAYAREA_HEIGHT - BALL_SIZE;
		ballSpeedY = -ballSpeedY;  // Bounce off bottom wall
	}

	// limit the area where we search for block collisions
	int8_t startCol = (int8_t)floor((ballX - (PLAYAREA_WIDTH - BLOCKS_COLUMNS * BLOCK_WIDTH)) / BLOCK_WIDTH);
	if (startCol < 0) {
		startCol = 0;
	}
	int8_t endCol = (int8_t)floor((ballX + BALL_SIZE - (PLAYAREA_WIDTH - BLOCKS_COLUMNS * BLOCK_WIDTH)) / BLOCK_WIDTH);
	if (endCol >= BLOCKS_COLUMNS) {
		endCol = BLOCKS_COLUMNS - 1;
	}
	int8_t startRow = (int8_t)floor(ballY / BLOCK_HEIGHT);
	if (startRow < 0) {
		startRow = 0;
	}
	int8_t endRow = (int8_t)floor((ballY + BALL_SIZE) / BLOCK_HEIGHT);
	if (endRow >= BLOCKS_ROWS) {
		endRow = BLOCKS_ROWS - 1;
	}
	// Check for block collisions
	for (uint8_t row = startRow; row <= endRow; row++) {
		for (uint8_t col = startCol; col <= endCol; col++) {
			if (!blocks[row][col]) {
				continue;
			}
			uint8_t blockX = col * BLOCK_WIDTH + PLAYAREA_WIDTH - BLOCKS_COLUMNS * BLOCK_WIDTH;
			uint8_t blockY = row * BLOCK_HEIGHT;

			if (ballX + BALL_SIZE < blockX || ballX > blockX + BLOCK_WIDTH ||
				ballY + BALL_SIZE < blockY || ballY > blockY + BLOCK_HEIGHT) {
				continue;  // No collision with this block
			}

			blocks[row][col] = false;  // Mark block as hit
			blockCount--;
			if (blockCount == 0) {
				gameWon = true;	 // All blocks hit, player won
				return;
			}

			// We calculate the anmount of overlap in both x and y direction
			// The smaller overlap will always be the direction of the bounce
			float blockMX = blockX + BLOCK_WIDTH / 2.0f;
			float blockMY = blockY + BLOCK_HEIGHT / 2.0f;
			float ballMX = ballX + BALL_SIZE / 2.0f;
			float ballMY = ballY + BALL_SIZE / 2.0f;

			uint8_t difX = (blockMX > ballMX) ? (ballX + BALL_SIZE - blockX) : (blockX - ballX);
			uint8_t difY = (blockMY > ballMY) ? (ballY + BALL_SIZE - blockY) : (blockY - ballY);

			// besides determining the direction of the bounce, we also adjust the ball position
			// to prevent it from hitting direct neighboring blocks in the same tick
			if (difX < difY) {
				// Ball is hitting the block from the left or right
				ballSpeedX = -ballSpeedX;
				// Adjust ball position
				if (blockMX > ballMX) {
					// Ball is on the left side of the block
					ballX = blockX - BALL_SIZE - 0.01f;
				} else {
					// Ball is on the right side of the block
					ballX = blockX + BLOCK_WIDTH + 0.01f;
				}
			} else {
				// Ball is hitting the block from the top or bottom
				ballSpeedY = -ballSpeedY;
				// Adjust ball position
				if (blockMY > ballMY) {
					// Ball is on the top side of the block
					ballY = blockY - BALL_SIZE - 0.01f;
				} else {
					// Ball is on the bottom side of the block
					ballY = blockY + BLOCK_HEIGHT + 0.01f;
				}
			}
		}
	}

	// Read joystick input
	float jy = (float)joystickRead();
	// Center the joystick value around 0
	jy -= 512.0f;
	// only move the platform if the joystick is not in deadzone
	if (abs(jy) > 80.0f) {
		jy /= (512.0f / 2.5f);	// Scale the joystick value to a range of -4 to 4

		// Update platform position
		platformY += jy;
		if (platformY < 0) {
			platformY = 0;
		} else if (platformY > PLAYAREA_HEIGHT - PLATFORM_SIZE - 1) {
			platformY = PLAYAREA_HEIGHT - PLATFORM_SIZE - 1;
		}
	}

	// Check for platform collisions
	if (ballX < 1) {
		// -1 / +1 to give a bit more leeway on the platform
		if (ballY + BALL_SIZE >= platformY - 2 && ballY <= platformY + PLATFORM_SIZE + 2) {
			// Bounce off platform
			ballX = 1;
			// calculate rebound angle
			float rad = -(platformY + PLATFORM_SIZE / 2.0f - ballY + BALL_SIZE / 2.0f);
			rad /= ((PLATFORM_SIZE + 4) / 2.0f);		  // Normalized to -1 to 1 (added 4 because of the leeway)
			rad *= (M_PI * 0.4f);						  // Scaled to a reasonable angle range
			rad = clamp(rad, -M_PI / 3.3f, M_PI / 3.3f);  // Clamp to prevent too steep angles
			// calculate new speed based on rebound angle
			ballSpeedX = BALL_VELOCITY * cos(rad);
			ballSpeedY = BALL_VELOCITY * sin(rad);
		}
	}
	if (ballX < 0) {
		// Ball is out of bounds on the left side
		lifes--;
		if (lifes == 0) {
			gameLost = true;
			return;
		}
		initBall();	 // Reset ball
	} else if (ballX > PLAYAREA_WIDTH - BALL_SIZE) {
		ballX = PLAYAREA_WIDTH - BALL_SIZE;
		ballSpeedX = -ballSpeedX;  // Bounce off right wall
	}
}

void gameDraw() {
	displayClearBuffer();

	if (gameWon || gameLost) {
		if (gameWon) {
			displayRenderTextVertical(DISPLAY_WIDTH / 2 + 7, 19, "You");
			displayRenderTextVertical(DISPLAY_WIDTH / 2 - 7, 15, "Won!");
		} else {
			displayRenderTextVertical(DISPLAY_WIDTH / 2 + 7, 15, "Game");
			displayRenderTextVertical(DISPLAY_WIDTH / 2 - 7, 11, "Over!");
		}

		displayUpdate();

		// disable further updates
		cli();

		return;
	}

	for (uint8_t i = 0; i < lifes; i++) {  // life bar
		displayDrawFilledRectangle(PLAYAREA_WIDTH + 2, i * PLAYAREA_HEIGHT / PLAYER_LIFES_START, LIFE_BAR_WIDTH, PLAYAREA_HEIGHT / PLAYER_LIFES_START);
	}

	displayDrawRectangle(0, round(platformY) + 1, 1, PLATFORM_SIZE);  // platform
	displayDrawHorizontalLine(0, 0, PLAYAREA_WIDTH);				  // top wall
	displayDrawHorizontalLine(0, PLAYAREA_HEIGHT, PLAYAREA_WIDTH);	  // bottom wall
	displayDrawVerticalLine(PLAYAREA_WIDTH - 1, 0, PLAYAREA_HEIGHT);  // right wall

	for (uint8_t row = 0; row < BLOCKS_ROWS; row++) {  // blocks
		for (uint8_t col = 0; col < BLOCKS_COLUMNS; col++) {
			if (blocks[row][col]) {
				displayDrawRectangle(col * BLOCK_WIDTH + PLAYAREA_WIDTH - BLOCKS_COLUMNS * BLOCK_WIDTH, row * BLOCK_HEIGHT + 1, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1);
			}
		}
	}

	displayDrawRectangle((uint8_t)ballX, (uint8_t)ballY + 1, BALL_SIZE, BALL_SIZE);	 // ball

	displayUpdate();
}

ISR(TIMER0_COMP_vect) {
	gameUpdate();
	gameDraw();
}

int main() {
	displaySetup();
	joystickInit();

	initBlocks();
	initBall();

	// Initialize Timer0 for game updates at 60Hz
	BIT_SET(TCCR0, WGM00);	// Set WGM00 bit for CTC mode
	BIT_CLR(TCCR0, WGM01);	// Clear WGM01 bit for CTC mode

	// Set Compare Match value
	OCR0 = (F_CPU / 1024 / 60) - 1;	 // (F_CPU / prescaler / frequency) - 1

	// Enable Timer0 Compare Match Interrupt
	BIT_SET(TIMSK, OCIE0);

	// Start Timer0 with 1024 prescaler
	TCCR0 |= (1 << CS02) | (1 << CS00);

	sei();

	while (1);	// waiting for the heatdeath of the universe
}
