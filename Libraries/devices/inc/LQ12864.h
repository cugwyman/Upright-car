#ifndef __LQ12864_H
#define __LQ12864_H

#include "root.h"
#include "stdint.h"

#define		OLED_DC_PORT	HW_GPIOE
#define		OLED_DC_PIN		3
#define		OLED_RST_PORT	HW_GPIOE
#define		OLED_RST_PIN	2
#define		OLED_SDA_PORT	HW_GPIOE
#define		OLED_SDA_PIN	0
#define		OLED_SCL_PORT	HW_GPIOE
#define		OLED_SCL_PIN	1


//--------------------定义IO端口-----------------//
#define LCD_DC(x)   GPIO_WriteBit(OLED_DC_PORT,OLED_DC_PIN,x)
#define LCD_RST(x)  GPIO_WriteBit(OLED_RST_PORT,OLED_RST_PIN,x)
#define LCD_SCL(x)	GPIO_WriteBit(OLED_SCL_PORT,OLED_SCL_PIN,x)
#define LCD_SDA(x)	GPIO_WriteBit(OLED_SDA_PORT,OLED_SDA_PIN,x)

#define X_WIDTH 128
#define Y_WIDTH 64

 void OLED_Init(void); 
 void OLED_CLS(void);
 void LCD_Fill(unsigned char bmp_data);
//extern void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
//extern void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
//extern void LCD_P14x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
//extern void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[]);
 void OLED_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);
//extern void Draw_LQLogo(void);
//extern void Draw_LibLogo(void);
 void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char * bmp); 
void LCD_PutPixel(unsigned char x,unsigned char y);
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor);
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void OLED_P6x8Num(unsigned char x,unsigned char y,uint16_t num);

#ifdef OLED
extern char oledBuf[8][50];
#endif

#endif

