#ifndef LCD_DRIVER_H

#define LCD_DRIVER_H

// LCD Dimensions
#define LCD_WIDTH       (96)
#define LCD_HEIGHT      (96) 

// 

struct CURSOR {
	uint8_t line;
	uint8_t row;
}Cursor;

struct GPS_LCD_DATA {
	double longitude;
	double latitude;
	int altitude;
	int velocity;
}GPS_DATA;

struct RUN_LCD_DATA {
	double dist;
	double pace;
	int timer;
}RUN_DATA;

struct TIMER_LCD_DATA {
	double lapTimes[4];
	int timer;
	int numLaps;
}TIMER_DATA;

struct TOP_BAR_DATA {
	int time;
	uint8_t batLevel;
	bool flag;
}TIMER_BAR;


//struct LCD {
 //public:
  //void LCD(uint8_t CLK, uint8_t MOSI, uint8_t SS);
  //void placeTemplate(int16_t x, int16_t y, uint16_t color);
  void clearDisplay();
  void clearLines(uint8_t start, uint8_t end);

  void refresh(void);

  void initCursor();

  void drawDottedLine(uint8_t line);

  //void writeStringToBitmap(char &s , int length);
  void transferChar(char c, uint8_t size);

  void transferSpecialChar(char c, uint8_t size);

  void transferSpecialLargeChar(char c);

  void setCursor(int x, int y);

  uint8_t bitmapToUint(int x, int y);

  void transferToBitmap(uint8_t data);
  void transferSmallNumInt(int num, uint8_t size);
  void transferBigNumInt(int num, uint8_t size);
  void transferSmallNumDouble(double num, uint8_t numSize, uint8_t size);
  void transferBigNumDouble(double num, uint8_t numSize, uint8_t size);
  void transferBatteryLevel(int num);

  void charTest();

  void initStructs();
  
  void buildGPS_LCD();
  void buildTimer_LCD();
  void buildRun_LCD();
  void buildTopBar_LCD();


 //private:
  //uint8_t ss, clk, mosi;
  uint8_t reverseBitOrder(uint8_t MSB);
//}LCD;


#endif