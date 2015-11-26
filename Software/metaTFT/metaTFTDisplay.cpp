/**
* definition for metaPixel TFT interface
*/
#include "metaTFTDisplay.h"
#include "Streaming.h"
void metaTFT::start()
{
  if(_isBacklightAnalog){
    analogWrite(_backlight_pin,255);
  }else{
    pinMode(_backlight_pin,OUTPUT);
    digitalWrite(_backlight_pin,_luminance);
  }
  this->begin();
  //delay(1000);
  this->setRotation(defaultRotation);
  this->fillScreen(ILI9341_BLACK);
  this->setTextWrap(true);
  drawLogo();

  if(_isBacklightAnalog){
    analogWrite(_backlight_pin,_luminance);
  }else{
   pinMode(_backlight_pin,OUTPUT);
    analogWrite(_backlight_pin,_luminance);
  }
  delay(2000);
}

void metaTFT::updateBacklight()
{
  if(_isBacklightAnalog){
    analogWrite(_backlight_pin,_luminance);
  }else{
    analogWrite(_backlight_pin,_luminance);
  }
}
static uint32_t fetchbits_unsigned(const uint8_t *p, uint32_t index, uint32_t required)
{
	uint32_t val = 0;
	do {
		uint8_t b = p[index >> 3];
		uint32_t avail = 8 - (index & 7);
		if (avail <= required) {
			val <<= avail;
			val |= b & ((1 << avail) - 1);
			index += avail;
			required -= avail;
		} else {
			b >>= avail - required;
			val <<= required;
			val |= b & ((1 << required) - 1);
			break;
		}
	} while (required);
	return val;
}
static uint32_t fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required)
{
	uint32_t val = fetchbits_unsigned(p, index, required);
	if (val & (1 << (required - 1))) {
		return (int32_t)val - (1 << required);
	}
	return (int32_t)val;
}

metaTFT::charDimentions metaTFT::fontCharDimentions(unsigned int c)
{
  uint32_t bitoffset;
  const uint8_t *data;
  metaTFT::charDimentions k={0,0};
  //Serial.printf("drawFontChar %d\n", c);

  if (c >= font->index1_first && c <= font->index1_last) {
    bitoffset = c - font->index1_first;
    bitoffset *= font->bits_index;
  } else if (c >= font->index2_first && c <= font->index2_last) {
    bitoffset = c - font->index2_first + font->index1_last - font->index1_first + 1;
    bitoffset *= font->bits_index;
  } else if (font->unicode) {
    return k; // TODO: implement sparse unicode
  } else {
    return k;
  }
  //Serial.printf("  index =  %d\n", fetchbits_unsigned(font->index, bitoffset, font->bits_index));
  data = font->data + fetchbits_unsigned(font->index, bitoffset, font->bits_index);

  uint32_t encoding = fetchbits_unsigned(data, 0, 3);
  if (encoding != 0) return k;
  //uint32_t width = fetchbits_unsigned(data, 3, font->bits_width);
  bitoffset = font->bits_width + 3;
  //uint32_t height = fetchbits_unsigned(data, bitoffset, font->bits_height);
  bitoffset += font->bits_height;
  //int32_t xoffset = fetchbits_signed(data, bitoffset, font->bits_xoffset);
  bitoffset += font->bits_xoffset;
  int32_t yoffset = fetchbits_signed(data, bitoffset, font->bits_yoffset);
  bitoffset += font->bits_yoffset;
//Serial.printf("  offset = %d,%d\n", xoffset, yoffset);

  uint32_t delta = fetchbits_unsigned(data, bitoffset, font->bits_delta);
	bitoffset += font->bits_delta;
  //Serial<<"size["<<((char)c)<<"] = "<<width<<", "<< height<<" ("<<delta<<")"<<"Xof: "<<xoffset<<" yOff: "<<yoffset<<endl;
  k.width = delta;
  k.height = font->cap_height-yoffset;
  return k;
}

uint16_t metaTFT::stringWidth(const char * str)
{
  if(!font){
    return strlen(str)*6 * textsize;
  }else{
    size_t i=0;
    uint32_t width = 0;
    //uint32_t height= 0;
    while(str[i]!= 0x00){
      charDimentions k = fontCharDimentions(str[i]);
      width += k.width;
      ++i;
//      height +=k.height;
    }
    return width;
  }
  return 0;
}
uint16_t metaTFT::stringHeight(const char * str){
  if(!font){
    return 7*textsize;
  }else{
    size_t i=0;
//    uint32_t width = 0;
    uint32_t height= 0;
    while(str[i]!= 0x00){
      charDimentions k = fontCharDimentions(str[i]);
      ++i;
//      width += k.width;
      height= (height < k.height)?k.height:height;
    }
    return height;

  }
  return 0;
}


void metaTFT::drawLogo()
{
  this->setTextSize(5);
  this->setCursor(25,15);
  this->setTextColor(ILI9341_GREEN);
  *this<<"   meta"<<endl<<"  Display"<<endl;
  this->drawFastHLine(this->getCursorX(),this->getCursorY()+10,this->width(),ILI9341_GREEN);
  this->setTextColor(ILI9341_YELLOW);
  this->setTextSize(2);
  *this<<endl;
  TFT_LogoEnd =0; //tft.cursor_y;
}