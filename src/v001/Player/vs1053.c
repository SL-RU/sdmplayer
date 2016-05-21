#include "vs1053.h"

VS1053_parametric parametric;


static void vs1053_spiSpeed(uint8_t speed) // 0 - 400kHz; 1 - 25MHz
{
	//CHECK YOUR APB1 FREQ!!!
	int prescaler = SPI_BAUDRATEPRESCALER_256;

	if (speed == 0)
		prescaler = SPI_BAUDRATEPRESCALER_128;
	else if (speed == 1)
		prescaler = SPI_BAUDRATEPRESCALER_8;
	taskENTER_CRITICAL();
	VS1053_SPI.Init.BaudRatePrescaler = prescaler;
	HAL_SPI_Init(&VS1053_SPI);
	taskEXIT_CRITICAL();
}

uint8_t VSvolume = 60, VScurvolume = 60;


void vs1053_trans(uint8_t data)
{
	HAL_SPI_Transmit(&VS1053_SPI, &data, sizeof(data), 10);
}
uint8_t vs1053_txrx(uint8_t data)
{
	uint8_t rec;
	HAL_SPI_TransmitReceive(&VS1053_SPI, &data, &rec, sizeof(data), 10);
	return rec;
}
//Write to VS10xx register
//SCI: Data transfers are always 16bit. When a new SCI operation comes in 
//DREQ goes low. We then have to wait for DREQ to go high again.
//XCS should be low for the full duration of operation.
void vs1053_write_reg(uint8_t addressbyte, uint8_t highbyte, uint8_t lowbyte){
	taskENTER_CRITICAL();
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET); //Wait for DREQ to go high indicating IC is available	
	HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_RESET); //Select control
  //SCI consists of instruction byte, address byte, and 16-bit data word.
  vs1053_trans(0x02); //Write instruction
  vs1053_trans(addressbyte);
  vs1053_trans(highbyte);
  vs1053_trans(lowbyte);
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET); //Wait for DREQ to go high indicating command is complete
	HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_SET); //Deselect Control
	taskEXIT_CRITICAL();
}

//Read the 16-bit value of a VS10xx register
uint16_t vs1053_read_reg (uint8_t addressbyte){
	taskENTER_CRITICAL();
	while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET); //Wait for DREQ to go high indicating IC is available
  HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_RESET); //Select control

	
  //SCI consists of instruction byte, address byte, and 16-bit data word.
  vs1053_trans(0x03); //Write instruction
  vs1053_trans(addressbyte);

  uint8_t response1 = vs1053_txrx(0xFF); //Read the first byte
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET); //Wait for DREQ to go high indicating command is complete
  uint8_t response2 = vs1053_txrx(0xFF); //Read the second byte
  while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET); //Wait for DREQ to go high indicating command is complete

	HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_SET); //Deselect Control
	taskEXIT_CRITICAL();
	
  uint16_t resultvalue = response1 << 8;
  resultvalue |= response2;
  return resultvalue;
}
uint16_t vs1053_write_reg_16 (uint16_t addressbyte, uint16_t val)
{
	vs1053_write_reg(addressbyte, val >> 8, val);
}
uint16_t vs1053_read_wram_16 (uint16_t addressbyte)
{
	vs1053_write_reg_16(SCI_WRAMADDR, addressbyte);
	uint16_t h = vs1053_read_reg(SCI_WRAM);
	return h;
}
void vs1053_write_wram_16 (uint16_t addressbyte, uint16_t val)
{
	vs1053_write_reg_16(SCI_WRAMADDR, addressbyte);
	vs1053_write_reg_16(SCI_WRAM, val);
}
uint32_t vs1053_read_wram_32 (uint8_t addressbyte)
{	
	//datasheet 10.11 Extra Parameters
	uint16_t h = vs1053_read_wram_16(addressbyte),
						l = vs1053_read_wram_16(addressbyte + 1);
	
	uint16_t h1 = vs1053_read_wram_16(addressbyte),
						l1 = vs1053_read_wram_16(addressbyte);
	
	if((h == h1 && l == l1) //doesn't changed - valid
	 ||	(h != h1 && l != l1) //both words changed - valid
		)
		return l + (h << 16);
	else //not valid - again
		return vs1053_read_wram_32(addressbyte);
	
}
void vs1053_write_wram_32 (uint8_t addressbyte, uint32_t val){
	vs1053_write_wram_16(addressbyte, val >> 16);	
	vs1053_write_wram_16(addressbyte + 1, val);	
}
void sine_test()
{	
	while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) osDelay(1); //Wait for DREQ to go high indicating IC is available
  HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_RESET); //Select control
	
	uint8_t data[] = {0x53, 0xef, 0x6e, 0x44, 0x00, 0x00, 0x00, 0x00};
	
	uint8_t i = 0;
	for(i = 0; i<8; i++)
		vs1053_trans(data[i]);
	
	while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET) osDelay(1); //Wait for DREQ to go high indicating IC is available
  HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_SET); //Select control
}

void set_vol(uint8_t vol)
{
	VSvolume = 10 + (100 - vol)*130/100;
}
void vs1053_read_parametric(void)
{
	parametric.version = vs1053_read_wram_16(PAR_VERSION);
	parametric.config1 = vs1053_read_wram_16(PAR_CONFIG1);
	parametric.playSpeed = vs1053_read_wram_16(PAR_PLAY_SPEED);
	
	slog("par %d %d %d", parametric.version, parametric.config1, parametric.playSpeed);
}
void VS1053_Init(void)
{
	printf("VS1053 initing\n");
	HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_RESET);
	VS1053_rstD();
	osDelay(1);
	slog("1");
	VS1053_rstU();
	HAL_GPIO_WritePin(VS1053_xCS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_SET);
	osDelay(1);
	slog("2");
	VS1053_rstD();
	
	vs1053_spiSpeed(0);
	vs1053_trans(0xFF);
	
	osDelay(10);
	slog("3");
	VS1053_rstU();
	
	vs1053_write_reg(SCI_VOL, 60, 60);
	slog("4");
	int MP3Mode = vs1053_read_reg(SCI_MODE);
	int MP3Status = vs1053_read_reg(SCI_STATUS);
	int MP3Clock = vs1053_read_reg(SCI_CLOCKF);


  printf("SCI_Mode (0x4800) = 0x%x\n", MP3Mode);

  printf("SCI_Status (0x48) = 0x%x\n", MP3Status);

  int vsVersion = (MP3Status >> 4) & 0x000F; //Mask out only the four version bits
  printf("VS Version (VS1053 is 4) = 0x%x\n", vsVersion); //The 1053B should respond with 4. VS1001 = 0, VS1011 = 1, VS1002 = 2, VS1003 = 3

  printf("SCI_ClockF = 0x%x\n", MP3Clock);
	vs1053_write_reg(SCI_CLOCKF, 0x60, 0x00); //set clock x3
	
	vs1053_read_parametric();
	
	//printf("sine\n");
	//vs1053_write_reg(SCI_MODE, 0x08, 0x20);
	//printf("SCI_Mode (0x4800) = 0x%x\n", vs1053_read_reg(SCI_MODE));
	vs1053_spiSpeed(1);
	//sine_test();
	//printf("done\n");

}

void vs1053_update_parametric(void)
{
	parametric.byteRate = vs1053_read_wram_16(PAR_VERSION);
	parametric.endFillByte = vs1053_read_wram_16(PAR_CONFIG1);
	parametric.playSpeed = vs1053_read_wram_16(PAR_PLAY_SPEED);
}

char* curfile_name;
uint8_t cancel = 0;
void VS1053_play_file(FIL* file, char* name)
{
	printf("play\n");
	cancel = 0;
	UINT br;
	uint8_t need = 1;
	
	curfile_name = name;
	
	
	uint8_t buf[32] = {0};

	uint32_t lst =  HAL_GetTick(), cur, vv = 0;
	
	vs1053_write_reg_16(SCI_DECODE_TIME, 0); //reset time
	
	while(1)
	{
		HAL_GetTick();
		while(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET)
		{
			if(need)
			{
				taskENTER_CRITICAL();
				if(f_read(file, &buf, 32, &br) != FR_OK)
				{
					cancel = 1;
					need = 0;
					break;				
				}
				taskEXIT_CRITICAL();
				cur = HAL_GetTick();
				lst = cur;
				need = 0;
				vv++;
				vs1053_update_parametric();
			}
			if(VSvolume != VScurvolume)
			{
				vs1053_spiSpeed(0);
				vs1053_write_reg(SCI_VOL, VSvolume, VSvolume);
				VScurvolume = VSvolume;
				slog("vols %d", VScurvolume);
				vs1053_spiSpeed(1);
			}
			else
				osDelay(1);
		}			//Wait for DREQ to go high indicating IC is available
		taskENTER_CRITICAL();
		if(need)
		{
			if(f_read(file, &buf, 32, &br) != FR_OK)
				break;
			cur = HAL_GetTick();
			lst = cur;
			vv++;
			need = 0;
		}
		if(cancel)
		{
			printf("canc\n");
			cancel = 0;
			break;
		}
		HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_RESET); //Select control
		HAL_SPI_Transmit(&VS1053_SPI, buf, 32, 1);      //trasmit data
		HAL_GPIO_WritePin(VS1053_xDCS, GPIO_PIN_SET);   //Select control
		taskEXIT_CRITICAL();
		need = 1;
	}
	curfile_name = 0;
	printf("end\n");
}
void VS1053_draw(void)
{
	if(curfile_name != 0)
		gui_lable(curfile_name, 0, 20, 128, 20, 1, 1);
}
uint8_t VS1053_input(uint8_t key)
{
	cancel = 1;
	gui_closeMessage();
	return 1;
}

uint32_t lst = 0;
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if( lst == 0)
//		lst = HAL_GetTick();
//	if(HAL_GPIO_ReadPin(VS1053_DREQ) == GPIO_PIN_RESET)
//	{
//		slog("tm:%d",  HAL_GetTick() - lst);
//		lst = HAL_GetTick();
//	}
//}
