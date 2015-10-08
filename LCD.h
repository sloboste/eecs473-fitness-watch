#include <stdint>

Class LCDdata{
    public:
	bool edit[128];
	bool img[16384];
   	void pushImage();
   	void writeData(bool * img, bool * edit, bool display, bool EXTCOMIN);
}

/*	Stores the bit map of what will be pushed to the screen
 */
bool img[16384];

/*	Keeps track of which lines of the image have been edited so that only the sections of the screen requiring edits will be edited. This is to save 
 *	CPU time, and refresh rate
 */
bool edit[128];
 
/*	Writes rows of the bit map designated by the edit array, to the LCD through use of SPI
 */
void writeData(uint8_t addr, bool display, bool EXTCOMIN);
 
/*	Calls writeData() for all the lines of the LCD that require updating
 */
void pushImage();