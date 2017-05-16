// 
// 
// 

#include "Song.h"


SongClass::SongClass(const uint8_t BPM, const uint16_t length, const Chord* score){
    this->position = 0;
    this->BPM = BPM;
    this->length = length;
    this->score = score;
}

Chord SongClass::next() {
    Chord nextval = this->score[this->position];
    this->position++;
    this->position %= length;
    return nextval;
}

