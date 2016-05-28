#ifndef VS1053_H
#define VS1053_H
#include "spi.h"
#include "gpio.h"
#include "vs1053_defs.h"


#define VS1053_rstD() HAL_GPIO_WritePin(VS1053_xRST, GPIO_PIN_RESET);
#define VS1053_rstU() HAL_GPIO_WritePin(VS1053_xRST, GPIO_PIN_SET)

#define VS1053_END_FILL_BYTES    2050
#define VS1053_MAX_TRANSFER_SIZE 32

#define PARAMETRIC_VERSION 0x0003
typedef struct {
/* configs are not cleared between files */
uint16_t version; /*1e02 - structure version */
uint16_t config1; /*1e03 ---- ---- ppss RRRR PS mode, SBR mode, Reverb */
uint16_t playSpeed; /*1e04 0,1 = normal speed, 2 = twice, 3 = three times etc. */
uint16_t byteRate; /*1e05 average byterate */
uint16_t endFillByte; /*1e06 byte value to send after file sent */
uint16_t reserved[16]; /*1e07..15 file byte offsets */
uint32_t jumpPoints[8]; /*1e16..25 file byte offsets */
uint16_t latestJump; /*1e26 index to lastly updated jumpPoint */
uint32_t positionMsec; /*1e27-28 play position, if known (WMA, Ogg Vorbis) */
int16_t resync; /*1e29 > 0 for automatic m4a, ADIF, WMA resyncs */
union {
struct {
uint32_t curPacketSize;
uint32_t packetSize;
} wma;
struct {
uint16_t sceFoundMask; /*1e2a SCE’s found since last clear */
uint16_t cpeFoundMask; /*1e2b CPE’s found since last clear */
uint16_t lfeFoundMask; /*1e2c LFE’s found since last clear */
uint16_t playSelect; /*1e2d 0 = first any, initialized at aac init */
int16_t dynCompress; /*1e2e -8192=1.0, initialized at aac init */
int16_t dynBoost; /*1e2f 8192=1.0, initialized at aac init */
uint16_t sbrAndPsStatus; /*0x1e30 1=SBR, 2=upsample, 4=PS, 8=PS active */
} aac;
struct {
uint32_t bytesLeft;
} midi;
struct {
uint16_t gain; /* 0x1e2a proposed gain offset in 0.5dB steps, default = -12 */
} vorbis;
} i;
} VS1053_parametric;

extern VS1053_parametric parametric;

#define PLAYER_STOP 0
#define PLAYER_PLAY 1
#define PLAYER_PAUSE 2
typedef struct {
	uint8_t appID; //Application for player
	char* shortName; //3char name
	uint8_t (*start)(void); //player started
	uint8_t (*stop)(void);  //player stoped
	void (*playComplete)(void); //current song completed
	void (*drawMenu)(void);        //draw info in menu
	uint8_t (*input_handlerMenu)(int8_t, uint32_t); //handle input in menu
} PlayerInfo;

#include <stdio.h>
#include "ff.h"
#include "keyboard.h"
#include "gui.h"

uint8_t VS1053_set_player(PlayerInfo* player);
PlayerInfo* VS1053_get_player(void);

uint8_t VS1053_getState(void);
void VS1053_pause(void);
void VS1053_unpause(void);

void set_vol(uint8_t vol);
void VS1053_Init(void);
void VS1053_play_file(FIL* file);



void VS1053_draw(void);
uint8_t VS1053_input(uint8_t key);

void VS1053_thread(void  * argument);

void vs1053_update_parametric(void);

#endif
