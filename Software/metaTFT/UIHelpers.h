/**
* a here the ui elements go
*/

#ifndef __UIHELPERS_H__
#define __UIHELPERS_H__
#include "Arduino.h"
#include <vector>
#include "metaTFTDisplay.h"

using namespace std;

class metaButton
{
public:

  metaButton(void):_gfx(NULL),_x(-1),_y(-1),_w(0),_h(0),_textsize(1) {}
	void initButton(metaTFT *gfx, int16_t x, int16_t y,
		uint8_t w, uint8_t h,
		uint16_t outline, uint16_t fill, uint16_t textcolor,
		const char *label, uint8_t textsize);
	void drawButton(bool inverted = false);
	bool contains(int16_t x, int16_t y);
	void press(boolean p) {
		laststate = currstate;
		currstate = p;
	}
	bool isPressed() { return currstate; }
	bool justPressed() { return (currstate && !laststate); }
	bool justReleased() { return (!currstate && laststate); }

	void setTextColor(uint16_t tc){_textcolor = tc;}
	uint16_t getTextColor(){return _textcolor;}

	void setFillColor(uint16_t fc){_fillcolor = fc;}
	uint16_t getFillColor(){return _fillcolor;}

	void setOutlineColor(uint16_t oc){_outlinecolor = oc;}
	uint16_t getOutlineColor(){return _outlinecolor;}

	int16_t getWidth(){return _w;}
	int16_t getHeight(){return _h;}
	int16_t getX(){return _x;}
	int16_t getY(){return _y;}

private:
	metaTFT *_gfx;
	int16_t _x, _y;
	uint16_t _w, _h;
	uint8_t _textsize;
	uint16_t _outlinecolor, _fillcolor, _textcolor;
	char _label[10];
	boolean currstate, laststate;
};

/** baseclass for UI elements */

class metaView
{

public:
  metaView(void):_gc(NULL),_frame(),_outlineColor(0),_backgroundColor(0),_cornerRadius(0),_opaque(true),
  _drawsOutline(false),_needsRedraw(false),_needsLayout(false) {};
  metaView(GCRect frame):_gc(NULL),_frame(frame),_outlineColor(0),_backgroundColor(0),_opaque(true),
  _drawsOutline(false),_needsRedraw(true),_needsLayout(true){};

  void initView(metaTFT* tft, GCRect frame);
  void initView(metaTFT* tft, GCPoint origin, GCSize size);
  void initView(metaTFT* tft, int16_t x, int16_t y, int16_t w, int16_t h);

  void setOutlineColor(uint16_t c){_outlineColor = c;_needsRedraw = true;};
  uint16_t getOutlineColor(){return _outlineColor;};

  void setDrawsOutline(boolean flag){_drawsOutline = flag;_needsRedraw = true;};
  boolean getDrawsOutline(){return _drawsOutline;};

  void setBackgroundColor(uint16_t c){_backgroundColor = c;_needsRedraw=true;};
  uint16_t getBackgroundColor(){return _backgroundColor;};

  void setOpaque(boolean f){_opaque = f;_needsRedraw=true;};
  boolean getOpaque(){return _opaque;};

  void setCornerRadius(uint8_t r){_cornerRadius = r;_needsRedraw=true;};
  uint8_t getCornerRadius(){return _cornerRadius;};

  void setSize(GCSize s){_frame.size = s;_needsLayout = true;};
  void setSize(int16_t w, int16_t h){setSize(GCSize(w,h));};
  GCSize getSize(){return _frame.size;};

  void setOrigin(GCPoint p){_frame.origin = p;_needsLayout=true;};
  void setOrigin(int16_t x, int16_t y){setOrigin(GCPoint(x,y));};
  GCPoint getOrigin(){return _frame.origin;};

  void addSubview(metaView* ptr);
  void removeSubview(metaView* subView);
  void removeFromSuperview();
  void setNeedsRedraw(){_needsRedraw = true;};
  void setNeedsLayout(){_needsLayout = true;};
  virtual GCSize intrinsicSize(){return GCSize();};
  virtual void redraw();
protected:
  const vector<metaView*>::iterator findSubview(metaView* subView);
  boolean childNeedsLayout();
  void resetFlags(){
    _needsLayout = false;
    _needsRedraw = false;
  };
  GraphicsContext _gc;
  GCRect _frame;
	uint16_t _outlineColor, _backgroundColor;  // colorization
  uint8_t _cornerRadius;                     // cornerradius
  boolean _opaque;                           // draws background color
  boolean _drawsOutline;
  boolean _needsRedraw;                       // triggers a redraw of the View
  boolean _needsLayout;                       // triggers a redraw of the superView;
  metaView* _superView;
  vector<metaView*> _subViews;
};

/** @todo: rework */
#define HALLIGN_LEFT    (1<<1);
#define HALLIGN_CENTER  (1<<2);
#define HALLIGN_RIGHT   (1<<3);
#define VALLIGN_TOP     (1<<4);
#define VALLIGN_CENTER  (1<<5);
#define VALLIGN_BOTTOM  (1<<6);

class metaLabel : public metaView
{
public:
  metaLabel(void):metaView(),_textColor(ILI9341_GREEN),_allignmentMask(),_textPosition(),_textSize(3),_font(NULL),_label(NULL){};
  metaLabel(const String* label,uint16_t textColor=ILI9341_GREEN):metaView(),_textColor(textColor),_allignmentMask(),_textPosition(),_textSize(3),_font(NULL),_label(label){};
  void setFont(const ILI9341_t3_font_t &f){_font = &f;_needsRedraw=true;};
  const ILI9341_t3_font_t *getFont(){return _font;};

  void setTextColor(uint16_t tc){_textColor=tc;_needsRedraw=true;};
  uint16_t getTextColor(){return _textColor;};

  void setTextSize(uint8_t s){_textSize = s;_needsRedraw=true;};
  uint8_t getTextSize(){return _textSize;};

  void setLabel(String* label){_label = label;_needsRedraw=true;};

  void setAllignmentMask(uint8_t mask){_allignmentMask = mask;};

  virtual void redraw();
  virtual GCSize intrinsicSize();
protected:
  uint16_t _textColor;        // textColor
  uint8_t  _allignmentMask;   // mask for TextAlignment;
  GCPoint  _textPosition;     // used if allignmentMask == 0
  uint8_t _textSize;          // used if font == NULL
  const ILI9341_t3_font_t *_font;   //
  const String *_label;
};
#endif
