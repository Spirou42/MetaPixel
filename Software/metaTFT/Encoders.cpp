/**
* Encoder Implementation
*/

#include "Encoders.h"

Encoder encoders[] = {
  Encoder(UserEvent::EncoderID::Encoder0,&eventQueue,19,20,0,2),
};
size_t numberOfEncoders = sizeof(encoders) / sizeof(Encoder);

void encoder0Handler();

EncoderIRQHandler encoderHandlers[] = {
  encoder0Handler,
};

void enableEncoders(){
  for(size_t i = 0;i<numberOfEncoders;++i){
    encoders[i].enable();
  }
}

void disableEncoders(){
  for(size_t i=0;i<numberOfEncoders;++i){
    encoders[i].disable();
  }
}

void encoderHandler(size_t idx){
  encoders[idx].doEncoderStep();
}

void encoder0Handler(){
  __disable_irq();
  encoderHandler(0);
  __enable_irq();
}
