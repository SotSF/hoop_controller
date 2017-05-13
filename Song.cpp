// 
// 
// 

#include "Song.h"


SongClass::SongClass(const uint8_t BPM, const uint16_t length, const chord* score){
	this->position = 0;
	this->BPM = BPM;
	this->length = length;
	this->score = score;
}

chord SongClass::next() {
	chord nextval = this->score[this->position];
	this->position++;
	this->position %= length;
	return nextval;
}

