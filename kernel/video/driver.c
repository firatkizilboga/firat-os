#pragma once

#include "video.h"
#include <stdbool.h>

VGATextFrame frame;
void * requestVideoOut(){
    return &setFrameBuffer;
};

void setFrameBuffer(VGATextFrame frame){

};

void initializeVideo(){

}

void videoInterruptHandler(){
    
}

