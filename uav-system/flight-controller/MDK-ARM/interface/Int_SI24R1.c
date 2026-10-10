#include "Int_SI24R1.h"

// ¶¨ÒåÒ»¸ö¾²Ì¬µÄ·¢ËÍµØÖ·  => ·¢ËÍµØÖ·Óë½ÓÊÕµØÖ·ÏàÍ¬
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x06, 0x1E, 0x01}; // ¶¨ÒåÒ»¸ö¾²Ì¬·¢ËÍµØÖ·

// SPI¶ÁĞ´Ò»¸ö×Ö½Ú => Ğ´ÈëµÄ×Ö½ÚÊÇ´«ÈëµÄ²ÎÊı  ¶ÁÈ¡µÄ×Ö½ÚÊÇ·µ»ØÖµ
static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t rx_data = 0;
	HAL_SPI_TransmitReceive(&hspi1, &byte, &rx_data, 1, 1000);
	return rx_data;
}

/********************************************************
º¯Êı¹¦ÄÜ£ºĞ´¼Ä´æÆ÷µÄÖµ£¨µ¥×Ö½Ú£©
Èë¿Ú²ÎÊı£ºreg:¼Ä´æÆ÷Ó³ÉäµØÖ·£¨¸ñÊ½£ºSI24R1_WRITE_REG£üreg£©
					value:¼Ä´æÆ÷µÄÖµ
·µ»Ø  Öµ£º×´Ì¬¼Ä´æÆ÷µÄÖµ
*********************************************************/
uint8_t Int_SI24R1_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;

	CS_LOW;
	status = SPI_RW(reg);
	SPI_RW(value);
	CS_HIGH;

	return (status);
}

/********************************************************
º¯Êı¹¦ÄÜ£ºĞ´¼Ä´æÆ÷µÄÖµ£¨¶à×Ö½Ú£©
Èë¿Ú²ÎÊı£ºreg:¼Ä´æÆ÷Ó³ÉäµØÖ·£¨¸ñÊ½£ºSI24R1_WRITE_REG£üreg£©
					pBuf:Ğ´Êı¾İÊ×µØÖ·
					bytes:Ğ´Êı¾İ×Ö½ÚÊı
·µ»Ø  Öµ£º×´Ì¬¼Ä´æÆ÷µÄÖµ
*********************************************************/
uint8_t Int_SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t size)
{
	uint8_t status, byte_ctr;

	CS_LOW;
	status = SPI_RW(reg);
	for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
	{
		SPI_RW(*pBuf++);
	}

	CS_HIGH;

	return (status);
}

/********************************************************
º¯Êı¹¦ÄÜ£º¶ÁÈ¡¼Ä´æÆ÷µÄÖµ£¨µ¥×Ö½Ú£©
Èë¿Ú²ÎÊı£ºreg:¼Ä´æÆ÷Ó³ÉäµØÖ·£¨¸ñÊ½£ºSI24R1_READ_REG£üreg£©
·µ»Ø  Öµ£º¼Ä´æÆ÷Öµ
*********************************************************/
uint8_t Int_SI24R1_Read_Reg(uint8_t reg)
{
	uint8_t value;

	CS_LOW;
	SPI_RW(reg);
	value = SPI_RW(0);
	CS_HIGH;

	return (value);
}

/********************************************************
º¯Êı¹¦ÄÜ£º¶ÁÈ¡¼Ä´æÆ÷µÄÖµ£¨¶à×Ö½Ú£©
Èë¿Ú²ÎÊı£ºreg:¼Ä´æÆ÷Ó³ÉäµØÖ·£¨SI24R1_READ_REG£üreg£©
					pBuf:½ÓÊÕ»º³åÇøµÄÊ×µØÖ·
					bytes:¶ÁÈ¡×Ö½ÚÊı
·µ»Ø  Öµ£º×´Ì¬¼Ä´æÆ÷µÄÖµ
*********************************************************/
uint8_t Int_SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size)
{
	uint8_t status, byte_ctr;

	CS_LOW;
	status = SPI_RW(reg);
	for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
	{
		pBuf[byte_ctr] = SPI_RW(0); // ¶ÁÈ¡Êı¾İ£¬µÍ×Ö½ÚÔÚÇ°
	}
	CS_HIGH;

	return (status);
}

/********************************************************
º¯Êı¹¦ÄÜ£ºSI24R1½ÓÊÕÄ£Ê½³õÊ¼»¯
Èë¿Ú²ÎÊı£ºÎŞ
·µ»Ø  Öµ£ºÎŞ
*********************************************************/
void Int_SI24R1_RX_Mode(void)
{
	CE_LOW;
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // ½ÓÊÕÉè±¸½ÓÊÕÍ¨µÀ0Ê¹ÓÃºÍ·¢ËÍÉè±¸ÏàÍ¬µÄ·¢ËÍµØÖ·
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);						   // Ê¹ÄÜ½ÓÊÕÍ¨µÀ0×Ô¶¯Ó¦´ğ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);					   // Ê¹ÄÜ½ÓÊÕÍ¨µÀ0
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, CHANNEL);					   // Ñ¡ÔñÉäÆµÍ¨µÀ40
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);			   // ½ÓÊÕÍ¨µÀ0Ñ¡ÔñºÍ·¢ËÍÍ¨µÀÏàÍ¬ÓĞĞ§Êı¾İ¿í¶È
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);					   // Êı¾İ´«ÊäÂÊ1Mbps£¬·¢Éä¹¦ÂÊ4dBm
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0f);						   // CRCÊ¹ÄÜ£¬16Î»CRCĞ£Ñé£¬ÉÏµç£¬½ÓÊÕÄ£Ê½
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, 0xff);						   // Çå³ıËùÓĞµÄÖĞ¶Ï±êÖ¾Î»
	CE_HIGH;																	   // À­¸ßCEÆô¶¯½ÓÊÕÉè±¸
}

/********************************************************
º¯Êı¹¦ÄÜ£ºSI24R1·¢ËÍÄ£Ê½³õÊ¼»¯
Èë¿Ú²ÎÊı£ºÎŞ
·µ»Ø  Öµ£ºÎŞ
*********************************************************/
void Int_SI24R1_TX_Mode(void)
{
	CE_LOW;
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	   // Ğ´Èë·¢ËÍµØÖ·
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // ÎªÁËÓ¦´ğ½ÓÊÕÉè±¸£¬½ÓÊÕÍ¨µÀ0µØÖ·ºÍ·¢ËÍµØÖ·ÏàÍ¬
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);						   // Ê¹ÄÜ½ÓÊÕÍ¨µÀ0×Ô¶¯Ó¦´ğ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);					   // Ê¹ÄÜ½ÓÊÕÍ¨µÀ0
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + SETUP_RETR, 0x0a);					   // ×Ô¶¯ÖØ·¢ÑÓÊ±µÈ´ı250us+86us£¬×Ô¶¯ÖØ·¢10´Î
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, CHANNEL);					   // Ñ¡ÔñÉäÆµÍ¨µÀ0x40
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);					   // Êı¾İ´«ÊäÂÊ1Mbps£¬·¢Éä¹¦ÂÊ4dBm
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0e);						   // CRCÊ¹ÄÜ£¬16Î»CRCĞ£Ñé£¬ÉÏµç
	CE_HIGH;
}

/********************************************************
º¯Êı¹¦ÄÜ£º¶ÁÈ¡½ÓÊÕÊı¾İ   Ó²¼şÖ±½Ó½ÓÊÕÊı¾İ±£´æµ½ FIFO¶ÓÁĞÖĞ => Í¨¹ı×´Ì¬±êÖ¾Î»ÅĞ¶Ï¶ÓÁĞÖĞÊÇ·ñÓĞÊı¾İ
Èë¿Ú²ÎÊı£ºrxbuf:½ÓÊÕÊı¾İ´æ·ÅÊ×µØÖ·
·µ»Ø  Öµ£º0:½ÓÊÕµ½Êı¾İ
		  1:Ã»ÓĞ½ÓÊÕµ½Êı¾İ
*********************************************************/
uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
{
	uint8_t state;
	// ½«¶ÁÈ¡µ½µÄÖµ Ô­·â²»¶¯ÔÙĞ´»Ø×´Ì¬¼Ä´æÆ÷  => ÒòÎª×´Ì¬¼Ä´æÆ÷ÖĞµÄ±êÖ¾Î»Éè¼ÆÎªĞ´1Çå³ı
	state = Int_SI24R1_Read_Reg(STATUS);					// ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // Çå³ıRX_DSÖĞ¶Ï±êÖ¾

	if (state & RX_DR) // ½ÓÊÕµ½Êı¾İ
	{
		Int_SI24R1_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH); // ¶ÁÈ¡Êı¾İ
		Int_SI24R1_Write_Reg(FLUSH_RX, 0xff);					 // Çå³ıRX FIFO¼Ä´æÆ÷
		return 0;
	}
	return 1; // Ã»ÊÕµ½ÈÎºÎÊı¾İ
}

/********************************************************
º¯Êı¹¦ÄÜ£º·¢ËÍÒ»¸öÊı¾İ°ü
Èë¿Ú²ÎÊı£ºtxbuf:Òª·¢ËÍµÄÊı¾İ
·µ»Ø  Öµ: 0: ·¢ËÍ³É¹¦ 1: ·¢ËÍÊ§°Ü
*********************************************************/
uint8_t Int_SI24R1_TxPacket(uint8_t *txbuf)
{
	uint8_t state;
	CE_LOW;													  // CEÀ­µÍ£¬Ê¹ÄÜSI24R1ÅäÖÃ
	Int_SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // Ğ´Êı¾İµ½TX FIFO,32¸ö×Ö½Ú
	CE_HIGH;												  // CEÖÃ¸ß£¬Ê¹ÄÜ·¢ËÍ

	// Ã»ÓĞÊ¹ÓÃÖĞ¶ÏÅĞ¶ÏÊÇ·ñ·¢ËÍÍê³É  => Ê¹ÓÃÂÖÑ¯¶ÁÈ¡×´Ì¬±êÖ¾Î»
	// while (IRQ == 1)
	// 	;										 // µÈ´ı·¢ËÍÍê³É

	state = Int_SI24R1_Read_Reg(STATUS); // ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ
	while (((state & TX_DS) == 0) && ((state & MAX_RT) == 0))
	{
		state = Int_SI24R1_Read_Reg(STATUS);
		vTaskDelay(1);
	}

	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // Çå³ıTX_DS»òMAX_RTÖĞ¶Ï±êÖ¾
	if (state & MAX_RT)										// ´ïµ½×î´óÖØ·¢´ÎÊı
	{
		Int_SI24R1_Write_Reg(FLUSH_TX, 0xff); // Çå³ıTX FIFO¼Ä´æÆ÷
		return 1;
	}
	if (state & TX_DS) // ·¢ËÍÍê³É
	{
		return 0;
	}
	return 1; // ·¢ËÍÊ§°Ü
}

uint8_t si24r1_rx_buff[5] = {0};

/**
 * @brief SI24R1µÄ³õÊ¼»¯¼ì²â
 *
 * @return uint8_t  0:¼ì²â³É¹¦  1:¼ì²âÊ§°Ü
 */
uint8_t Int_SI24R1_Check(void)
{

	// 1. ²âÊÔSPIÍ¨ĞÅÄÜ¹»Õı³£¶ÁĞ´¼Ä´æÆ÷
	// 1.0 SI24R1Ğ¾Æ¬ĞèÒªÏÈ¶ÁÈ¡Ò»´Î  ±£Ö¤SPIÕı³£Ö®ºóÔÙĞ´Èë
	Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, si24r1_rx_buff, TX_ADR_WIDTH);

	// 1.1 Ğ´Èë·¢ËÍµØÖ·
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);

	// 1.2 ¶ÁÈ¡Í¬ÑùµÄÊı¾İ
	Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, si24r1_rx_buff, TX_ADR_WIDTH);

	for (uint8_t i = 0; i < TX_ADR_WIDTH; i++)
	{
		if (si24r1_rx_buff[i] != TX_ADDRESS[i])
		{
			return 1;
		}
	}
	return 0;
}

/**
 * @brief Ó²¼ş½Ó¿Ú²ãSI24R1µÄ³õÊ¼»¯
 *
 */
void Int_SI24R1_Init(void)
{
	// 0. å…ˆå½»åº•å¤ä½SI24R1ï¼šè¿›å…¥æ‰ç”µæ¨¡å¼ï¼Œæ¨¡æ‹Ÿå†·å¯åŠ¨
	CE_LOW;
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x00);  // PWR_UP=0
	HAL_Delay(5);
	Int_SI24R1_Write_Reg(FLUSH_TX, 0xff);
	Int_SI24R1_Write_Reg(FLUSH_RX, 0xff);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, 0xff);
	
	// 1. ä¸Šç”µå»¶è¿Ÿ >100ms
	// ÉÏµçÖ®ºóµÄĞ¾Æ¬ÑÓ³Ù >100ms
	HAL_Delay(200);
	// Ğ£Ñé¼ì²â
	while (Int_SI24R1_Check() == 1)
	{
		// Ã¿Á½´Î¼ì²â¼ä¸ô10ms
		HAL_Delay(10);
	}

	// ÉèÖÃÄ¬ÈÏµÄ×´Ì¬Îª½ÓÊÕÄ£Ê½  => Ã¿´Î·¢ËÍÊı¾İµÄÊ±ºò  ÇĞ»»µ½·¢ËÍ×´Ì¬ 
	Int_SI24R1_RX_Mode();
	printf("SI24R1 Init Success!\r\n");
}

