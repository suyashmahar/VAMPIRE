/*

COMMAND.CPP

VAMPIRE: Variation-Aware model of Memory Power Informed by Real Experiments
https://github.com/CMU-SAFARI/VAMPIRE

Copyright (c) SAFARI Research Group at Carnegie Mellon University and ETH Zürich
Released under the MIT License

*/

#include "command.h"
#include "helper.h"
#include <algorithm>
#include <iterator>

Command::Command(CommandType cmdType, const MappedAdd add, uint64_t issueTime, uint64_t finishTime, unsigned int data[16])
        : type(cmdType), issueTime(issueTime), finishTime(finishTime), add(add) {
    for (uint32_t i = 0; i < 16; i++) {
        this->data[i] = data[i];
    }
}

uint64_t Command::getIssueTime() const {
    return issueTime;
}

void Command::setIssueTime(uint64_t issueTime) {
    Command::issueTime = issueTime;
}

uint64_t Command::getFinishTime() const {
    return finishTime;
}

void Command::setFinishTime(uint64_t finishTime) {
    Command::finishTime = finishTime;
}

const MappedAdd &Command::getAdd() const {
    return add;
}

void Command::setAdd(const MappedAdd &add) {
    Command::add = add;
}

CommandType Command::getType() const {
    return type;
}

void Command::setType(CommandType type) {
    Command::type = type;
}

std::string Command::to_string() const {
    std::stringstream ss;
    ss << "type: " << commandString[int(this->type)]
         << ", issueTime: " << this->issueTime
         << ", finishTime: " << this->finishTime
         << ", add: " << add.to_string()
         << ", data: " << std::hex;

    for (int i = 0; i < 16; i++) {
        ss << this->data[i];
    }

    ss << std::dec;
    return ss.str();
}

std::ostream& operator<<(std::ostream& ostr, CommandType& cmdType) {
    ostr << commandString[static_cast<int>(cmdType)];
}

/* Outputs all the details of a command to an ostream object */
std::ostream& operator<<(std::ostream& ostr, Command& cmd) {
    ostr << cmd.to_string() << std::endl;
}