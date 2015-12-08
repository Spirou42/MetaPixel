#define FASTLED_INTERNAL
#include <stack>
#include <vector>
#include <tuple>
#include <list>
#include "Math.h"

#include "SPI.h"
#include "ILI9341_t3.h"
#include "Queue.h"
#include "Streaming.h"
#include "FastLED.h"

#include "UserEvent.h"
#include "metaTFTDisplay.h"
#include "Switch.h"
#include "Parameter.h"
#include "Switches.h"
#include "Encoder.h"
#include "Encoders.h"
#include "FastLED.h"
#include "Palettes.h"
#include "font_Arial.h"
#include "font_ArialBold.h"
#include "font_GillSans.h"
#include "font_GillSans_SemiBold.h"
#include "GraphicTests.h"
#include "UIHelpers.h"
//#include "font_Montserrat_Regular.h"
#include "LEDEffects.h"
#include "metaTFT.h"

CRGB  leds[NUM_LEDS];

metaTFT tft = metaTFT(TFT_CS, TFT_DC,TFT_RST,TFT_MOSI,TFT_SCK,TFT_MISO,TFT_LED,3);
UserEventQueue eventQueue = UserEventQueue();
using namespace std;

PaletteList initializeSystemPalettes(){
	PaletteList tmp;
	tmp.push_back(new PalettePair("Rainbow",RainbowColors_p));
	tmp.push_back(new PalettePair("Clouds",CloudColors_p));
	tmp.push_back(new PalettePair("Ocean",OceanColors_p));
	tmp.push_back(new PalettePair("Forest",ForestColors_p));
	tmp.push_back(new PalettePair("Party",PartyColors_p));
	tmp.push_back(new PalettePair("Rainbow Stripes",RainbowStripeColors_p));
	tmp.push_back(new PalettePair("Lava",LavaColors_p));
	tmp.push_back(new PalettePair("Heat",HeatColors_p));
	tmp.push_back(new PalettePair("Arctic",arctic_gp));
	tmp.push_back(new PalettePair("Temperature",temperature_gp));
	tmp.push_back(new PalettePair("Colombia",colombia_gp));
	tmp.push_back(new PalettePair("Cequal",cequal_gp));
	return tmp;
}

EffectList initializeSystemEffects(){
	EffectList tmp;
	tmp.push_back(new EffectPair("Sinelon",&sinelon));
	tmp.push_back(new EffectPair("Rainbow",rainbow));
	tmp.push_back(new EffectPair("Rainbow Glitter",&rainbowWithGlitter));
	tmp.push_back(new EffectPair("Confetti",&confetti));
	tmp.push_back(new EffectPair("Juggle",&juggle));
	tmp.push_back(new EffectPair("BPM",&bpm));
	return tmp;
}

PaletteList systemPalettes = initializeSystemPalettes();
PaletteList::iterator currentSystemPalette = systemPalettes.begin();

EffectList systemEffects = initializeSystemEffects();
EffectList::iterator currentSystemEffect = systemEffects.begin();

ResponderStack responderStack;

Queue taskQueue;

/**					Global UI Elements				**/
metaList  SystemMenu;
metaView	SecondView;
metaList	EffectsMenu;
metaList 	PalettesMenu;
metaValue ValueView;

metaLabel::LabelLayout*  getListLayout(){
	static metaView::ViewLayout viewLayout;
	static metaLabel::LabelLayout labelLayout;
	static bool isInitialized = false;
	if(!isInitialized){
		viewLayout.backgroundColor=ILI9341_BLACK;
		viewLayout.outlineColor=ILI9341_ORANGE;
		viewLayout.opaque=false;
		labelLayout.viewLayout = &viewLayout;
		labelLayout.font = &Arial_14;
		labelLayout.insets=GCSize(5,5);
		labelLayout.indicatorSpace = 14;
		labelLayout.indicatorSize = GCSize(6,6);
		labelLayout.textSize=1;
		labelLayout.textColor=ILI9341_GREEN;
		viewLayout.visualizeState=true;
		isInitialized=true;
	}
	return &labelLayout;
}

metaValue::ValueLayout getValueLayout(){
	static metaValue::ValueLayout valueLayout;
	static bool isInitialized = false;
	if(!isInitialized){
		Serial << "getValueLayout" <<endl;
		valueLayout.labelFont = &Arial_16;
		valueLayout.valueFont = &Arial_40;
		valueLayout.labelOutlineCornerRadius = 5;
		valueLayout.labelOutlineInset = 3;
		valueLayout.labelDrawOutline=false;
		valueLayout.verticalValueInset=0;
		valueLayout.horizontalLabelInset=18;
		valueLayout.horizontalValueInset=0;
		valueLayout.valueColor = ILI9341_DARKGREEN;
		isInitialized = true;
	}
	return valueLayout;
}

void initListVisual(metaList &k){
	k.setBorderInset(GCSize(15,5));
	k.setLabelLayout(getListLayout());
	k.setDrawsOutline(true);
	k.setCornerRadius(3);
	k.setOutlineColor(ILI9341_RED);
	k.setOpaque(false);
}

void initializeTFT(){
	tft.start();
}

void initializeLEDs(){
	FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(COLOR_CORRECTION);
	FastLED.clear(true);
	FastLED.setBrightness( LED_BRIGHTNESS );
	FastLED.show();
}

void initSystemMenu(){
	// visual them definition for a single list entry

	SystemMenu.initView(&tft,GCRect(2,12,tft.width()/2,tft.height()-4));
	initListVisual(SystemMenu);

	SystemMenu.addEntry( String("Brightness"));
	SystemMenu.addEntry( String("Program"));
	SystemMenu.addEntry( String("Pallette"));
	SystemMenu.addEntry( String("Test"));
	SystemMenu.addEntry( String("Tast"));

	SystemMenu.layoutList();
	SystemMenu.sizeToFit();

	// Layout the second view
	GCPoint selO = SystemMenu.getOrigin();
	GCSize 	selS = SystemMenu.getSize();
	int seVx = selO.x+selS.w + 2;
	SecondView.initView(&tft,GCRect(seVx, selO.y,
	tft.width()-seVx-selO.x,selS.h));
	SecondView.setDrawsOutline(true);
	SecondView.setCornerRadius(3);
	SecondView.setOutlineColor(ILI9341_NAVY);
	SecondView.setOpaque(false);
}

void initEffectsMenu(){
	EffectsMenu.initView(&tft,GCRect(2,12,tft.width()/2,tft.height()-4));
	initListVisual(EffectsMenu);
	EffectList::iterator iter = systemEffects.begin();
	while(iter != systemEffects.end()){
		EffectsMenu.addEntry((*iter)->first );
		iter ++;
	}
	EffectsMenu.layoutList();
	EffectsMenu.sizeToFit();
}

void initPalettesMenu(){
	PalettesMenu.initView(&tft,GCRect(2,12,tft.width()/2,tft.height()-4));
	initListVisual(PalettesMenu);
	PaletteList::iterator iter = systemPalettes.begin();
	while(iter != systemPalettes.end()){
		PalettesMenu.addEntry((*iter)->first);
		iter ++;
	}
	PalettesMenu.layoutList();
	PalettesMenu.sizeToFit();
}

String labelStr = String("Brightness");

String valueStr = String("-55");


void initValueView(){
	Serial <<"initValueView"<<endl;
	ValueView.initValue(&tft,GCRect(130,00,13,8), labelStr, valueStr);

	metaValue::ValueLayout k = getValueLayout();

	ValueView.setLayout(k);

	ValueView.sizeToFit();
	ValueView.setProcessEvents(true);

	ValueView.allignInSuperView(HALLIGN_CENTER | VALLIGN_CENTER);
	valueStr.remove(0);
	valueStr+=String("10");
	ValueView.setValue(valueStr);

}


void initUI()
{
	initSystemMenu();
	initPalettesMenu();
	initEffectsMenu();
	initValueView();
}
elapsedMillis firstTime = elapsedMillis(0);

elapsedMillis displayTimer ;
elapsedMillis ledTimer;


int processLEDEffects(unsigned long now,void* data){
	if(ledTimer > (1000/FRAMES_PER_SECOND)){
		EffectPair *l = *currentSystemEffect;
		effectHandler h = l->second;
		h();
		//patterns[currentPatternNumber]();
		FastLED.show();
		EVERY_N_MILLISECONDS( 20 ) { gHue--; } // slowly cycle the "base color" through the rainbow
		EVERY_N_SECONDS( 120 ) { nextPattern(); } // change patterns periodically
		EVERY_N_SECONDS(30){nextPalette();}
	}
	return 0;
}


void adjustBrightness()
{
	int8_t uValue = log(256-tft.getLuminance())*10;
	tft.fillScreen(ILI9341_BLACK);

	String labelStr = String("Brightness");
	String valueStr = String("-55 ");
	String blubberStr = String ("-UU ");
	metaValue bla = metaValue();
	metaValue::ValueLayout brightnessTheme;
	brightnessTheme.labelFont = &Arial_16;
	brightnessTheme.valueFont = &Arial_40;
	brightnessTheme.labelOutlineCornerRadius = 10;
	brightnessTheme.labelOutlineInset = 8;
	brightnessTheme.labelDrawOutline=false;
	brightnessTheme.verticalValueInset=20;
	brightnessTheme.horizontalLabelInset=10;
	brightnessTheme.horizontalValueInset=30;
	brightnessTheme.valueColor = ILI9341_DARKGREEN;

	bla.setLayout(brightnessTheme);
	bla.initValue(&tft,GCRect(100,00,13,8), labelStr, valueStr);
	bla.sizeToFit();
	Serial << "Allign Now"<<endl;
	bla.allignInSuperView(HALLIGN_CENTER | VALLIGN_CENTER);
	valueStr.remove(0);
	valueStr+=String()+uValue;
	bla.setValue(valueStr);
	bla.redraw();

	static elapsedMillis lastAdjust = elapsedMillis(0);

	lastAdjust =0;

	do{
		taskQueue.Run(millis());
		if(eventQueue.length()){
			int8_t kValue = uValue;
			UserEvent *evnt = eventQueue.popEvent();
			if(evnt->getType()==EventType::EventTypeButton){
				ButtonData data = evnt->getData().buttonData;
				if(data.id==ButtonID::CenterButton &&
					data.state == ButtonState::ButtonClick){
					break;
				}
				if(data.id == ButtonID::UpButton &&
					(data.state == ButtonState::ButtonClick ||
						data.state == ButtonState::ButtonLongClick)){
					uValue = 55;
					lastAdjust=0;
				}
				if(data.id == ButtonID::DownButton &&
					(data.state == ButtonState::ButtonClick ||
						data.state == ButtonState::ButtonLongClick)){
					uValue = 0;
					lastAdjust=0;
				}

			}else if(evnt->getType() ==EventType::EventTypeEncoder){
				EncoderData data = evnt->getData().encoderData;
				int8_t steps = data.absSteps;
				uValue +=steps;
				lastAdjust=0;
			}
			if(uValue > 55){
				uValue = 55;
			}else if(uValue <0){
				uValue = 0;
			}
			if(uValue != kValue){
				uint8_t k= exp(uValue/10.0);
				k = 256 -k;
				tft.setLuminance(k);
				valueStr.remove(0);
				valueStr += String(uValue);
				Serial << "ValueStr: "<<valueStr<<endl;
				bla.setValue(valueStr);
				bla.valueUpdate();
				bla.redraw();
			//  tft.setCursor(20,20);
			//  tft.fillRect(20,80,16*6*tft.getTextSize(),7*tft.getTextSize(),ILI9341_BLUE);
			//  tft << "Brightness: "<<tft.getLuminance();
				//Serial <<"Brightness: "<<tft.getLuminance()<<"    "<<endl;
			}
		}
	}while(true/*lastg<5000*/);
}



/** todo: re-move this functionalityto a generic implementation */
int processUserEvents(unsigned long now, void * userdata){
	if(responderStack.size()==0){
		Serial << "There is no top responder"<<endl;
		return 0;
	}
	metaView *resp = responderStack.top();
	while(eventQueue.length()){
		UserEvent *evnt = eventQueue.popEvent();
		//uint16_t l = evnt->eventMask();
		uint16_t k = resp->respondsToEvents();
		int16_t oldSelectedElement =resp->selectedIndex();
		if((evnt->matchesMask(k)) ){
			int16_t result = resp->processEvent(evnt);
			if(result > ResponderResult::ChangedValue){
				Serial << "List changed Value x = "<<result<<endl;
				metaAction *a = resp->getAction();
				if(a){
					Serial << "Responder got a value action"<<endl;
				}else{
					Serial << "Responder did not have a value action"<<endl;
				}
				resp->redraw();
			}else{
				switch(result){
					case ResponderResult::ChangedNothing:
					Serial << "List did not change"<< endl;
					break;

					case ResponderResult::ChangedVisual:
					Serial << "List changed visualy"<< endl;
					resp->redraw();
					break;

					case ResponderResult::ChangedState:			/// this only is send if there was a list select
					{
						int16_t idx =resp->activeIndex();
						Serial << "List changed state selected "<<idx<<endl;
						metaAction *a = resp->getAction();
						if(a){
							Serial << "resp has a action for this"<< endl;
						}else{
							// check if the list entry has an metaAction added;
							metaView * p = resp->activeElement();
							if(p){
								metaAction *a = p->getAction();
								if(a){
									Serial << "got an action on the active Elements"<<endl;
								}else{
									Serial << "on element does not have a action"<<endl;
								}
							}
						}
						resp->redraw();
					}
					break;

					case ResponderResult::ResponderExit:
					Serial << "List will exit"<< endl;
					break;

				}
			}
		}
		delete evnt;
	}
	return 0;
}


void setup() {
	Serial.begin(115200);
	while(!Serial){}
	Serial << "Start"<<endl;
	Serial << "Effects: "<<systemEffects.size()<<endl;
	Serial << "Palettes: "<<systemPalettes.size()<<endl;
	initializeLEDs();

	// init LED Backlight
	pinMode(TFT_LED,OUTPUT);
	digitalWriteFast(TFT_LED,1);

	Serial << "Init TFT"<<endl;
	initializeTFT();

	// enable UI
	enableSwitches();
	enableEncoders();

	// draw mask
	initUI();

	// initialize tasks
	taskQueue.scheduleFunction(processLEDEffects,NULL,"EFFC",0,1000/FRAMES_PER_SECOND);
	taskQueue.scheduleFunction(processUserEvents,NULL,"USER",0,100);
}

bool skipMask = false;
void loop() {
	// put your main code here, to run repeatedly:
	if(firstTime>1000 && !skipMask){
		tft.fillScreen(ILI9341_BLACK);
		SecondView.redraw();
		responderStack.push(&ValueView);
		responderStack.top()->redraw();


		Serial << "Draw"<<endl;
		Serial.flush();
		skipMask = true;
	}
	if(responderStack.empty()){ // this is not good
		responderStack.push(&SystemMenu);
	}

	/** run all sequence tasks */
	taskQueue.Run(millis());

}
