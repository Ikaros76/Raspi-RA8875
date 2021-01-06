enum RA8875sizes {
  RA8875_480x80, RA8875_480x128, RA8875_480x272, RA8875_800x480
};

typedef struct {
  uint16_t	bitmapOffset;
  uint8_t	width;
  uint8_t	height;
  uint8_t	xAdvance;
  int8_t	xOffset;
  int8_t	yOffset;
} GFXglyph;

typedef struct {
  uint8_t	*bitmap;
  GFXglyph	*glyph;
  uint8_t	first;
  uint8_t	last;
  uint8_t	yAdvance;
} GFXfont;

class raspiRA8875 {

public:

  raspiRA8875(uint8_t cs, uint8_t rst, uint8_t spi_clock_div);
  unsigned long millis(void);
  void SPIBegin(void);
  bool displayBegin(enum RA8875sizes);
  void displayOn(bool on);
void writeData(uint8_t c);
void writeCommand(uint8_t c);
uint8_t readData(void);
void writeReg(const uint8_t reg, uint8_t val);
void waitBusy(uint8_t res);
bool waitPoll(uint8_t r, uint8_t f);

void textMode(void);
void graphicsMode(void);
void textWrite(const char *buffer);
void textEnlarge(uint8_t scale);
void setTextCursor(uint16_t x, uint16_t y);
void setTextColor(uint16_t foreColor, uint16_t bgColor);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void fillScreen(uint16_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
  void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
  void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                    int16_t y2, uint16_t color);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                    int16_t y2, uint16_t color);
  void drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis,
                   int16_t shortAxis, uint16_t color);
  void fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis,
                   int16_t shortAxis, uint16_t color);
  void drawCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis,
                 int16_t shortAxis, uint8_t curvePart, uint16_t color);
  void fillCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis,
                 int16_t shortAxis, uint8_t curvePart, uint16_t color);
  void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
                     uint16_t color);
  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
                     uint16_t color);

  /* Scroll */
  void setScrollWindow(int16_t x, int16_t y, int16_t w, int16_t h,
                       uint8_t mode);
  void scrollX(int16_t dist);
  void scrollY(int16_t dist);

  /* Backlight */
  void GPIOX(bool on);
  void PWM1config(bool on, uint8_t clock);
  void PWM2config(bool on, uint8_t clock);
  void PWM1out(uint8_t p);
  void PWM2out(uint8_t p);

void clearMemory(bool stop);
void clearActiveWindow(bool full);

void setFontGFX(const GFXfont *f = NULL);
void drawCharGFX(int16_t x, int16_t y, const char *c, uint16_t color, uint8_t size);

private:

int16_t applyRotationX(int16_t x);
int16_t applyRotationY(int16_t y);
void circleHelper(int16_t x, int16_t y, int16_t r, uint16_t color, bool filled);
void rectHelper(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled);
void triangleHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled);
void ellipseHelper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color, bool filled);
void curveHelper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color, bool filled);
void roundRectHelper(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color, bool filled);

  void swap(int16_t &x, int16_t &y) {
    int16_t temp = x;
    x = y;
    y = temp;
  }

  uint8_t  _scale = 0;
  uint8_t  _voffset;
  bool     _textMode = false;
  uint8_t  _cs, _rst;
  uint16_t _width, _height;
  uint8_t _rotation;
  enum RA8875sizes _size;

  GFXfont  *gfxFont;
};

// Colors (RGB565)
#define RA8875_BLACK 0x0000
#define RA8875_BLUE 0x001F
#define RA8875_RED 0xF800     ///< Red Color
#define RA8875_GREEN 0x07E0   ///< Green Color
#define RA8875_CYAN 0x07FF    ///< Cyan Color
#define RA8875_MAGENTA 0xF81F ///< Magenta Color
#define RA8875_YELLOW 0xFFE0  ///< Yellow Color
#define RA8875_WHITE 0xFFFF   ///< White Color

// Command/Data pins for SPI
#define RA8875_DATAWRITE 0x00
#define RA8875_DATAREAD 0x40
#define RA8875_CMDWRITE 0x80
#define RA8875_CMDREAD 0xC0

// Registers & bits
#define RA8875_PWRR 0x01          
#define RA8875_PWRR_DISPON 0x80   
#define RA8875_PWRR_DISPOFF 0x00  
#define RA8875_PWRR_SLEEP 0x02     
#define RA8875_PWRR_NORMAL 0x00    
#define RA8875_PWRR_SOFTRESET 0x01 

#define RA8875_MRWC 0x02 

#define RA8875_GPIOX 0xC7 

#define RA8875_PLLC1 0x88         
#define RA8875_PLLC1_PLLDIV2 0x80 
#define RA8875_PLLC1_PLLDIV1 0x00 

#define RA8875_PLLC2 0x89        
#define RA8875_PLLC2_DIV1 0x00   
#define RA8875_PLLC2_DIV2 0x01   
#define RA8875_PLLC2_DIV4 0x02   
#define RA8875_PLLC2_DIV8 0x03   
#define RA8875_PLLC2_DIV16 0x04  
#define RA8875_PLLC2_DIV32 0x05  
#define RA8875_PLLC2_DIV64 0x06  
#define RA8875_PLLC2_DIV128 0x07 

#define RA8875_SYSR 0x10       
#define RA8875_SYSR_8BPP 0x00  
#define RA8875_SYSR_16BPP 0x0C 
#define RA8875_SYSR_MCU8 0x00  
#define RA8875_SYSR_MCU16 0x03 

#define RA8875_PCSR 0x04       
#define RA8875_PCSR_PDATR 0x00 
#define RA8875_PCSR_PDATL 0x80 
#define RA8875_PCSR_CLK 0x00   
#define RA8875_PCSR_2CLK 0x01  
#define RA8875_PCSR_4CLK 0x02  
#define RA8875_PCSR_8CLK 0x03  

#define RA8875_HDWR 0x14 

#define RA8875_HNDFTR 0x15         
#define RA8875_HNDFTR_DE_HIGH 0x00 
#define RA8875_HNDFTR_DE_LOW 0x80  

#define RA8875_HNDR 0x16      
#define RA8875_HSTR 0x17      
#define RA8875_HPWR 0x18      
#define RA8875_HPWR_LOW 0x00  
#define RA8875_HPWR_HIGH 0x80 

#define RA8875_VDHR0 0x19     
#define RA8875_VDHR1 0x1A     
#define RA8875_VNDR0 0x1B     
#define RA8875_VNDR1 0x1C     
#define RA8875_VSTR0 0x1D     
#define RA8875_VSTR1 0x1E     
#define RA8875_VPWR 0x1F      
#define RA8875_VPWR_LOW 0x00  
#define RA8875_VPWR_HIGH 0x80 

#define RA8875_HSAW0 0x30 
#define RA8875_HSAW1 0x31 
#define RA8875_VSAW0 0x32 
#define RA8875_VSAW1 0x33 

#define RA8875_HEAW0 0x34 
#define RA8875_HEAW1 0x35 
#define RA8875_VEAW0 0x36 
#define RA8875_VEAW1 0x37 

#define RA8875_MCLR 0x8E            
#define RA8875_MCLR_START 0x80      
#define RA8875_MCLR_STOP 0x00       
#define RA8875_MCLR_READSTATUS 0x80 
#define RA8875_MCLR_FULL 0x00       
#define RA8875_MCLR_ACTIVE 0x40     

#define RA8875_DCR 0x90                   
#define RA8875_DCR_LINESQUTRI_START 0x80  
#define RA8875_DCR_LINESQUTRI_STOP 0x00   
#define RA8875_DCR_LINESQUTRI_STATUS 0x80 
#define RA8875_DCR_CIRCLE_START 0x40      
#define RA8875_DCR_CIRCLE_STATUS 0x40     
#define RA8875_DCR_CIRCLE_STOP 0x00       
#define RA8875_DCR_FILL 0x20              
#define RA8875_DCR_NOFILL 0x00            
#define RA8875_DCR_DRAWLINE 0x00          
#define RA8875_DCR_DRAWTRIANGLE 0x01      
#define RA8875_DCR_DRAWSQUARE 0x10        

#define RA8875_ELLIPSE 0xA0        
#define RA8875_ELLIPSE_STATUS 0x80 

#define RA8875_MWCR0 0x40         
#define RA8875_MWCR0_GFXMODE 0x00 
#define RA8875_MWCR0_TXTMODE 0x80 
#define RA8875_MWCR0_CURSOR 0x40  
#define RA8875_MWCR0_BLINK 0x20   

#define RA8875_MWCR0_DIRMASK 0x0C ///< Bitmask for Write Direction
#define RA8875_MWCR0_LRTD 0x00    ///< Left->Right then Top->Down
#define RA8875_MWCR0_RLTD 0x04    ///< Right->Left then Top->Down
#define RA8875_MWCR0_TDLR 0x08    ///< Top->Down then Left->Right
#define RA8875_MWCR0_DTLR 0x0C    ///< Down->Top then Left->Right

#define RA8875_BTCR 0x44  
#define RA8875_CURH0 0x46 
#define RA8875_CURH1 0x47 
#define RA8875_CURV0 0x48 
#define RA8875_CURV1 0x49 

#define RA8875_P1CR 0x8A         
#define RA8875_P1CR_ENABLE 0x80  
#define RA8875_P1CR_DISABLE 0x00 
#define RA8875_P1CR_CLKOUT 0x10  
#define RA8875_P1CR_PWMOUT 0x00  

#define RA8875_P1DCR 0x8B 

#define RA8875_P2CR 0x8C         
#define RA8875_P2CR_ENABLE 0x80  
#define RA8875_P2CR_DISABLE 0x00 
#define RA8875_P2CR_CLKOUT 0x10  
#define RA8875_P2CR_PWMOUT 0x00  

#define RA8875_P2DCR 0x8D 

#define RA8875_PWM_CLK_DIV1 0x00     
#define RA8875_PWM_CLK_DIV2 0x01     
#define RA8875_PWM_CLK_DIV4 0x02     
#define RA8875_PWM_CLK_DIV8 0x03     
#define RA8875_PWM_CLK_DIV16 0x04    
#define RA8875_PWM_CLK_DIV32 0x05    
#define RA8875_PWM_CLK_DIV64 0x06    
#define RA8875_PWM_CLK_DIV128 0x07   
#define RA8875_PWM_CLK_DIV256 0x08   
#define RA8875_PWM_CLK_DIV512 0x09   
#define RA8875_PWM_CLK_DIV1024 0x0A  
#define RA8875_PWM_CLK_DIV2048 0x0B  
#define RA8875_PWM_CLK_DIV4096 0x0C  
#define RA8875_PWM_CLK_DIV8192 0x0D  
#define RA8875_PWM_CLK_DIV16384 0x0E 
#define RA8875_PWM_CLK_DIV32768 0x0F 

#define RA8875_TPCR0 0x70               
#define RA8875_TPCR0_ENABLE 0x80        
#define RA8875_TPCR0_DISABLE 0x00       
#define RA8875_TPCR0_WAIT_512CLK 0x00   
#define RA8875_TPCR0_WAIT_1024CLK 0x10  
#define RA8875_TPCR0_WAIT_2048CLK 0x20  
#define RA8875_TPCR0_WAIT_4096CLK 0x30  
#define RA8875_TPCR0_WAIT_8192CLK 0x40  
#define RA8875_TPCR0_WAIT_16384CLK 0x50 
#define RA8875_TPCR0_WAIT_32768CLK 0x60 
#define RA8875_TPCR0_WAIT_65536CLK 0x70 
#define RA8875_TPCR0_WAKEENABLE 0x08    
#define RA8875_TPCR0_WAKEDISABLE 0x00   
#define RA8875_TPCR0_ADCCLK_DIV1 0x00   
#define RA8875_TPCR0_ADCCLK_DIV2 0x01   
#define RA8875_TPCR0_ADCCLK_DIV4 0x02   
#define RA8875_TPCR0_ADCCLK_DIV8 0x03   
#define RA8875_TPCR0_ADCCLK_DIV16 0x04  
#define RA8875_TPCR0_ADCCLK_DIV32 0x05  
#define RA8875_TPCR0_ADCCLK_DIV64 0x06  
#define RA8875_TPCR0_ADCCLK_DIV128 0x07 

#define RA8875_TPCR1 0x71            
#define RA8875_TPCR1_AUTO 0x00       
#define RA8875_TPCR1_MANUAL 0x40     
#define RA8875_TPCR1_VREFINT 0x00    
#define RA8875_TPCR1_VREFEXT 0x20    
#define RA8875_TPCR1_DEBOUNCE 0x04   
#define RA8875_TPCR1_NODEBOUNCE 0x00 
#define RA8875_TPCR1_IDLE 0x00       
#define RA8875_TPCR1_WAIT 0x01       
#define RA8875_TPCR1_LATCHX 0x02     
#define RA8875_TPCR1_LATCHY 0x03     

#define RA8875_TPXH 0x72  
#define RA8875_TPYH 0x73  
#define RA8875_TPXYL 0x74 

#define RA8875_INTC1 0xF0     
#define RA8875_INTC1_KEY 0x10 
#define RA8875_INTC1_DMA 0x08 
#define RA8875_INTC1_TP 0x04  
#define RA8875_INTC1_BTE 0x02 

#define RA8875_INTC2 0xF1     
#define RA8875_INTC2_KEY 0x10 
#define RA8875_INTC2_DMA 0x08 
#define RA8875_INTC2_TP 0x04  
#define RA8875_INTC2_BTE 0x02 

#define RA8875_SCROLL_BOTH 0x00   
#define RA8875_SCROLL_LAYER1 0x40 
#define RA8875_SCROLL_LAYER2 0x80 
#define RA8875_SCROLL_BUFFER 0xC0 

#define RA8875_DMACR 0xBF
