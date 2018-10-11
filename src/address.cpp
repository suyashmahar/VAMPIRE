/*

ADDRESS.CPP

VAMPIRE: Variation-Aware model of Memory Power Informed by Real Experiments
https://github.com/CMU-SAFARI/VAMPIRE

Copyright (c) SAFARI Research Group at Carnegie Mellon University and ETH Zürich
Released under the MIT License

*/

#include "address.h"

/*********************/
/* Class : MappedAdd */
/*********************/

MappedAdd::MappedAdd(unsigned long channel, unsigned long rank, unsigned long bank, unsigned long row,
                     unsigned long col) : channel(channel), rank(rank), bank(bank), row(row), col(col) {}
MappedAdd::MappedAdd(const MappedAdd &obj) {
    this->channel = obj.channel;
    this->rank = obj.rank;
    this->bank = obj.bank;
    this->row = obj.row;
    this->col = obj.col;
}
void MappedAdd::reset() {
    this->channel = 0ul;
    this->rank = 0ul;
    this->bank = 0ul;
    this->row = 0ul;
    this->col = 0ul;
}

std::string MappedAdd::to_string() const {
    std::stringstream ss;
    ss << "chan: " << this->channel
         << ", rank: " << this->rank
         << ", bank: " << this->bank
         << ", row: " << this->row
         << ", col: " << this->col;
    return ss.str();
}

std::ostream& operator<<(std::ostream& ostr, MappedAdd& add) {
    ostr << add.to_string();
}

/***********************/
/* Class : UnmappedAdd */
/***********************/

UnmappedAdd::UnmappedAdd(std::bitset<UNMAPPED_ADD_SIZE> *add) : add(add) {}

UnmappedAdd::~UnmappedAdd() {
    delete add;
}


