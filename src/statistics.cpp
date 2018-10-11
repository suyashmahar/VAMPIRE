/*

STATISTICS.CPP

VAMPIRE: Variation-Aware model of Memory Power Informed by Real Experiments
https://github.com/CMU-SAFARI/VAMPIRE

Copyright (c) 2018 SAFARI Research Group at Carnegie Mellon University and ETH Zürich
Released under the MIT License

*/

#include "statistics.h"
#include "vampire.h"

/***************************************/
/* Defining ScalarStat Class Functions */
/***************************************/

/* Constructor */
template<typename T>
ScalarStat<T>::ScalarStat(T value, std::string name, std::string unit, std::string description)
        :value(value), name(name), unit(unit), description(description) {}


template <typename T>
std::string ScalarStat<T>::toString() const {
    std::stringstream ss;
    ss << std::setw(40) << this->name << ": "
       << std::setw(20) << this->value
       << std::setw(5) << this->unit;
    if (this->description != "")
        ss << "  # " << this->description;
    ss << std::endl;
    return ss.str();
};

/* Getters and setters */
template<typename T>
void ScalarStat<T>::setValue(T value) {
    this->value = value;
}
template<typename T>
const std::string &ScalarStat<T>::getName() const {
    return name;
}
template<typename T>
void ScalarStat<T>::setName(const std::string &name) {
    this->name = name;
}
template<typename T>
const std::string &ScalarStat<T>::getUnit() const {
    return unit;
}
template<typename T>
void ScalarStat<T>::setUnit(const std::string &unit) {
    this->unit = unit;
}
template<typename T>
const std::string &ScalarStat<T>::getDescription() const {
    return description;
}
template<typename T>
void ScalarStat<T>::setDescription(const std::string &description) {
    this->description = description;
}

/***************************************/
/* Defining VectorStat Class Functions */
/***************************************/
template <typename T>
VectorStat<T>::VectorStat(uint64_t memberCount, T initialValue, std::string name, std::string unit, std::string description) {
    members = new std::vector<T>(memberCount, initialValue);
    this->name = name;
}

template <typename T>
VectorStat<T>::~VectorStat() {
    delete members;
}

/***************************************/
/* Defining Statistics Class Functions */
/***************************************/
void Statistics::print_stats() const {
    msg::info("Printing stats...");

    // Helper functions for printing vector stats
    std::function<std::string(uint64_t)> cmdToString=[&] (uint64_t index) -> std::string {return commandString[index] + " count";};
    std::function<std::string(uint64_t)> cmdToStringCycles=[&] (uint64_t index) -> std::string {return commandString[index] + " cycles";};

    auto interleavingToString=[&] (std::string parent, uint64_t index) -> std::string {return parent + "." + cmdInterleavingString[index];};
    auto rdInterleavingToString = std::bind(interleavingToString, "RD_Interleaving", std::placeholders::_1);
    auto wrInterleavingToString = std::bind(interleavingToString, "WR_Interleaving", std::placeholders::_1);

    std::cout
              << cmdCount->toString(cmdToString) << std::endl
              << cmdCycles->toString(cmdToStringCycles) << std::endl
              << totalCycleCount->toString() << std::endl
              << totalActStandbyCycles->toString()
              << totalPreStandbyCycles->toString() << std::endl
              << totalReadEnergy->toString()
              << totalWriteEnergy->toString()
              << totalPreCmdEnergy->toString()
              << totalActCmdEnergy->toString()
              << totalPrechargeStandbyEnergy->toString()
              << totalActiveStandbyEnergy->toString()
              << totalEnergy->toString() << std::endl

              << rdInterleavingCount->toString(rdInterleavingToString) << std::endl
              << wrInterleavingCount->toString(wrInterleavingToString) << std::endl

              << avgPower->toString()
              << avgCurrent->toString();
}

Statistics::Statistics(uint64_t (&structCount)[int(Level::MAX)]) {
    this->structCount = structCount;

    totalEnergy.reset(new ScalarStat<double_t>(
            0,
            "total energy",
            "pJ",
            ""
    ));
    totalReadEnergy.reset(new ScalarStat<double_t>(
            0ul,
            "totalReadEnergy",
            "pJ",
            "Estimated energy consumption by RD/RDA command excluding the active standby energy for the command duration."
    ));
    totalWriteEnergy.reset(new ScalarStat<double_t>(
            0ul,
            "totalWriteEnergy",
            "pJ",
            "Estimated energy consumption by WR/WRA command excluding the active standby energy for the command duration."
    ));
    totalActiveStandbyEnergy.reset(new ScalarStat<double_t>(
            0ul,
            "totalActiveStandbyEnergy",
            "pJ",
            "Total energy consumed by the DRAM when atleast one of the banks were active."
    ));
    totalPrechargeStandbyEnergy.reset(new ScalarStat<double_t>(
            0ul,
            "totalPrechargeStandbyEnergy",
            "pJ",
            "Total energy consumed by the DRAM when none of the banks were active."
    ));
    cmdCount.reset(new VectorStat<uint64_t>(
            (uint64_t) (CommandType::MAX),
            0ul,
            "Count of each command type",
            "",
            ""
    ));
    cmdCycles.reset(new VectorStat<uint64_t>(
            (uint64_t) (CommandType::MAX),
            0ul,
            "Cycle count of each command type",
            "",
            "Number of memory cycles spent on a command"
    ));
    totalActStandbyCycles.reset(new ScalarStat<uint64_t>(
            0ul,
            "totalActStandbyCycles",
            "",
            "Total number of cycles for which the rank was in active standby mode"
    ));
    totalPreStandbyCycles.reset(new ScalarStat<uint64_t>(
            0ul,
            "totalPreStandbyCycles",
            "",
            "Total number of cycles for which the rank was in precharge standby mode"
    ));
    totalActCmdEnergy.reset(new ScalarStat<double_t>(
            0ul,
            "totalActCmdEnergy",
            "pJ",
            ""
    ));
    totalPreCmdEnergy.reset(new ScalarStat<double_t>(
            0ul,
            "totalPreCmdEnergy",
            "pJ",
            ""
    ));
    totalCycleCount.reset(new ScalarStat<uint64_t>(
            0ul,
            "totalCycleCount",
            "",
            ""
    ));
    avgPower.reset(new ScalarStat<double_t>(
            0.0,
            "avgPower",
            "mW",
            ""
    ));
    avgCurrent.reset(new ScalarStat<double_t>(
            0.0,
            "avgCurrent",
            "mA",
            ""
    ));

    rdInterleavingCount.reset(new VectorStat<uint64_t>(
            static_cast<uint64_t>(CmdInterleaving::MAX),
            0,
            "rdInterleavingCount",
            "",
            ""
    ));
    wrInterleavingCount.reset(new VectorStat<uint64_t>(
            static_cast<uint64_t>(CmdInterleaving::MAX),
            0,
            "wrInterleavingCount",
            "",
            ""
    ));
}


void Statistics::calculateTotal(DramSpec &dramSpec, uint64_t endTime) {
    *this->totalEnergy = *this->totalActCmdEnergy
                         + *this->totalPreCmdEnergy
                         + *this->totalActiveStandbyEnergy
                         + *this->totalPrechargeStandbyEnergy
                         + *this->totalReadEnergy
                         + *this->totalWriteEnergy;
    this->totalEnergy->setName("totalEnergy");

    this->totalCycleCount->setValue(endTime);
    this->totalCycleCount->setName("totalCycleCount");
    this->avgPower->setValue(totalEnergy->getValue()/(2.5*this->totalCycleCount->getValue()));
    this->avgCurrent->setValue(this->avgPower->getValue()/dramSpec.vdd);
}
