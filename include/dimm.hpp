/*
// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#pragma once
#include "smbios.hpp"
#include <xyz/openbmc_project/Inventory/Decorator/Asset/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/Dimm/server.hpp>
#include <xyz/openbmc_project/Association/Definitions/server.hpp>
#include <xyz/openbmc_project/Inventory/Connector/Slot/server.hpp>
#include <xyz/openbmc_project/Inventory/Decorator/LocationCode/server.hpp>
#include <xyz/openbmc_project/Inventory/Item/server.hpp>
#include <xyz/openbmc_project/State/Decorator/OperationalStatus/server.hpp>

namespace phosphor
{

namespace smbios
{

#ifdef SMBIOS_MDRV1

using DeviceType =
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Dimm::DeviceType;

class Dimm :
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Inventory::Item::server::Dimm>,
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Inventory::Decorator::server::Asset>,
    sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Inventory::
                                    Decorator::server::LocationCode>,
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Inventory::Connector::server::Slot>,
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Inventory::server::Item>,
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Association::server::Definitions>,
    sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::State::
                                    Decorator::server::OperationalStatus>

{
  public:
    Dimm() = delete;
    ~Dimm() = default;
    Dimm(const Dimm &) = delete;
    Dimm &operator=(const Dimm &) = delete;
    Dimm(Dimm &&) = default;
    Dimm &operator=(Dimm &&) = default;

    Dimm(sdbusplus::bus_t& bus, const std::string& objPath,
         const uint8_t& dimmId, struct ManagedDataRegion *region) :

        sdbusplus::server::object_t<
            sdbusplus::xyz::openbmc_project::Inventory::Item::server::Dimm>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Inventory::
                                        Decorator::server::Asset>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Inventory::
                                        Decorator::server::LocationCode>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Inventory::
                                        Connector::server::Slot>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<
            sdbusplus::xyz::openbmc_project::Inventory::server::Item>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<
            sdbusplus::xyz::openbmc_project::Association::server::Definitions>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::State::
                                        Decorator::server::OperationalStatus>(
            bus, objPath.c_str()),
        dimmNum(dimmId), regionS(region)
    {
        memoryInfoUpdate();
    }

    void memoryInfoUpdate(void);

    uint16_t memoryDataWidth(uint16_t value) override;
    uint32_t memorySizeInKB(uint32_t value) override;
    std::string memoryDeviceLocator(std::string value) override;
    DeviceType memoryType(DeviceType value) override;
    std::string memoryTypeDetail(std::string value) override;
    uint16_t maxMemorySpeedInMhz(uint16_t value) override;
    std::string manufacturer(std::string value) override;
    bool present(bool value) override;
    std::string serialNumber(std::string value) override;
    std::string partNumber(std::string value) override;
    std::string locationCode(std::string value) override;
    uint8_t memoryAttributes(uint8_t value) override;
    uint16_t memoryConfiguredSpeedInMhz(uint16_t value) override;
    bool functional(bool value) override;

  private:
    uint8_t dimmNum;

    struct ManagedDataRegion *regionS;

    void dimmSize(uint16_t size);
    void dimmSizeExt(uint32_t size);
    void dimmDeviceLocator(uint8_t positionNum, uint8_t structLen,
                           uint8_t *dataIn);
    void dimmType(uint8_t type);
    void dimmTypeDetail(uint16_t detail);
    void dimmManufacturer(uint8_t positionNum, uint8_t structLen,
                          uint8_t *dataIn);
    void dimmSerialNum(uint8_t positionNum, uint8_t structLen, uint8_t *dataIn);
    void dimmPartNum(uint8_t positionNum, uint8_t structLen, uint8_t *dataIn);

    struct MemoryInfo
    {
        uint8_t type;
        uint8_t length;
        uint16_t handle;
        uint16_t phyArrayHandle;
        uint16_t errInfoHandle;
        uint16_t totalWidth;
        uint16_t dataWidth;
        uint16_t size;
        uint8_t formFactor;
        uint8_t deviceSet;
        uint8_t deviceLocator;
        uint8_t bankLocator;
        uint8_t memoryType;
        uint16_t typeDetail;
        uint16_t speed;
        uint8_t manufacturer;
        uint8_t serialNum;
        uint8_t assetTag;
        uint8_t partNum;
        uint8_t attributes;
        uint32_t extendedSize;
        uint16_t confClockSpeed;
    } __attribute__((packed));
};

const std::map<uint8_t, DeviceType> dimmTypeTable = {
    {0x1, DeviceType::Other},         {0x2, DeviceType::Unknown},
    {0x3, DeviceType::DRAM},          {0x4, DeviceType::EDRAM},
    {0x5, DeviceType::VRAM},          {0x6, DeviceType::SRAM},
    {0x7, DeviceType::RAM},           {0x8, DeviceType::ROM},
    {0x9, DeviceType::FLASH},         {0xa, DeviceType::EEPROM},
    {0xb, DeviceType::FEPROM},        {0xc, DeviceType::EPROM},
    {0xd, DeviceType::CDRAM},         {0xe, DeviceType::ThreeDRAM},
    {0xf, DeviceType::SDRAM},         {0x10, DeviceType::DDR_SGRAM},
    {0x11, DeviceType::RDRAM},        {0x12, DeviceType::DDR},
    {0x13, DeviceType::DDR2},         {0x14, DeviceType::DDR2_SDRAM_FB_DIMM},
    {0x18, DeviceType::DDR3},         {0x19, DeviceType::FBD2},
    {0x1a, DeviceType::DDR4},         {0x1b, DeviceType::LPDDR_SDRAM},
    {0x1c, DeviceType::LPDDR2_SDRAM}, {0x1d, DeviceType::LPDDR3_SDRAM},
    {0x1e, DeviceType::LPDDR4_SDRAM}, {0x1f, DeviceType::Logical},
    {0x20, DeviceType::HBM},          {0x21, DeviceType::HBM2}};

const std::string detailTable[16]{
    "Reserved",      "Other",         "Unknown",     "Fast-paged",
    "Static column", "Pseudo-static", "RAMBUS",      "Synchronous",
    "CMOS",          "EDO",           "Window DRAM", "Cache DRAM",
    "Non-volatile",  "Registered",    "Unbuffered",  "LRDIMM"};

#elifdef SMBIOS_MDRV2

using DeviceType =
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Dimm::DeviceType;

using EccType =
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::Dimm::Ecc;

class Dimm :
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Inventory::Item::server::Dimm>,
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Inventory::Decorator::server::Asset>,
    sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Inventory::
                                    Decorator::server::LocationCode>,
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Inventory::Connector::server::Slot>,
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Inventory::server::Item>,
    sdbusplus::server::object_t<
        sdbusplus::xyz::openbmc_project::Association::server::Definitions>,
    sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::State::
                                    Decorator::server::OperationalStatus>
{
  public:
    Dimm() = delete;
    ~Dimm() = default;
    Dimm(const Dimm&) = delete;
    Dimm& operator=(const Dimm&) = delete;
    Dimm(Dimm&&) = default;
    Dimm& operator=(Dimm&&) = default;

    Dimm(sdbusplus::bus_t& bus, const std::string& objPath,
         const uint8_t& dimmId, uint8_t* smbiosTableStorage,
         const std::string& motherboard) :

        sdbusplus::server::object_t<
            sdbusplus::xyz::openbmc_project::Inventory::Item::server::Dimm>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Inventory::
                                        Decorator::server::Asset>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Inventory::
                                        Decorator::server::LocationCode>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::Inventory::
                                        Connector::server::Slot>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<
            sdbusplus::xyz::openbmc_project::Inventory::server::Item>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<
            sdbusplus::xyz::openbmc_project::Association::server::Definitions>(
            bus, objPath.c_str()),
        sdbusplus::server::object_t<sdbusplus::xyz::openbmc_project::State::
                                        Decorator::server::OperationalStatus>(
            bus, objPath.c_str()),
        dimmNum(dimmId), storage(smbiosTableStorage),
        motherboardPath(motherboard)
    {
        memoryInfoUpdate();
    }

    void memoryInfoUpdate(void);

    uint16_t memoryDataWidth(uint16_t value) override;
    size_t memorySizeInKB(size_t value) override;
    std::string memoryDeviceLocator(std::string value) override;
    DeviceType memoryType(DeviceType value) override;
    std::string memoryTypeDetail(std::string value) override;
    uint16_t maxMemorySpeedInMhz(uint16_t value) override;
    std::string manufacturer(std::string value) override;
    bool present(bool value) override;
    std::string serialNumber(std::string value) override;
    std::string partNumber(std::string value) override;
    std::string locationCode(std::string value) override;
    uint8_t memoryAttributes(uint8_t value) override;
    uint16_t memoryConfiguredSpeedInMhz(uint16_t value) override;
    bool functional(bool value) override;
    EccType ecc(EccType value) override;

  private:
    uint8_t dimmNum;

    uint8_t* storage;

    std::string motherboardPath;

    void dimmSize(const uint16_t size);
    void dimmSizeExt(const size_t size);
    void dimmDeviceLocator(const uint8_t bankLocatorPositionNum,
                           const uint8_t deviceLocatorPositionNum,
                           const uint8_t structLen, uint8_t* dataIn);
    void dimmType(const uint8_t type);
    void dimmTypeDetail(const uint16_t detail);
    void dimmManufacturer(const uint8_t positionNum, const uint8_t structLen,
                          uint8_t* dataIn);
    void dimmSerialNum(const uint8_t positionNum, const uint8_t structLen,
                       uint8_t* dataIn);
    void dimmPartNum(const uint8_t positionNum, const uint8_t structLen,
                     uint8_t* dataIn);
    void updateEccType(uint16_t exPhyArrayHandle);
};

struct MemoryInfo
{
    uint8_t type;
    uint8_t length;
    uint16_t handle;
    uint16_t phyArrayHandle;
    uint16_t errInfoHandle;
    uint16_t totalWidth;
    uint16_t dataWidth;
    uint16_t size;
    uint8_t formFactor;
    uint8_t deviceSet;
    uint8_t deviceLocator;
    uint8_t bankLocator;
    uint8_t memoryType;
    uint16_t typeDetail;
    uint16_t speed;
    uint8_t manufacturer;
    uint8_t serialNum;
    uint8_t assetTag;
    uint8_t partNum;
    uint8_t attributes;
    size_t extendedSize;
    uint16_t confClockSpeed;
    uint16_t minimumVoltage;
    uint16_t maximumVoltage;
    uint16_t configuredVoltage;
    uint8_t memoryTechnology;
    uint16_t memoryOperatingModeCap;
    uint8_t firwareVersion;
    uint16_t modelManufId;
    uint16_t modelProdId;
    uint16_t memSubConManufId;
    uint16_t memSubConProdId;
    uint64_t nvSize;
    uint64_t volatileSize;
    uint64_t cacheSize;
    uint64_t logicalSize;
} __attribute__((packed));

/**
 * @brief Struct to represent SMBIOS 3.2 type-16 (Physical Memory Array) data.
 */
struct PhysicalMemoryArrayInfo
{
    uint8_t type;
    uint8_t length;
    uint16_t handle;
    uint8_t location;
    uint8_t use;
    uint8_t memoryErrorCorrection;
    uint32_t maximumCapacity;
    uint16_t memoryErrorInformationHandle;
    uint16_t numberOfMemoryDevices;
    uint64_t extendedMaximumCapacity;
} __attribute__((packed));
static_assert(sizeof(PhysicalMemoryArrayInfo) == 23,
              "Size of PhysicalMemoryArrayInfo struct is incorrect.");

const std::map<uint8_t, DeviceType> dimmTypeTable = {
    {0x1, DeviceType::Other},         {0x2, DeviceType::Unknown},
    {0x3, DeviceType::DRAM},          {0x4, DeviceType::EDRAM},
    {0x5, DeviceType::VRAM},          {0x6, DeviceType::SRAM},
    {0x7, DeviceType::RAM},           {0x8, DeviceType::ROM},
    {0x9, DeviceType::FLASH},         {0xa, DeviceType::EEPROM},
    {0xb, DeviceType::FEPROM},        {0xc, DeviceType::EPROM},
    {0xd, DeviceType::CDRAM},         {0xe, DeviceType::ThreeDRAM},
    {0xf, DeviceType::SDRAM},         {0x10, DeviceType::DDR_SGRAM},
    {0x11, DeviceType::RDRAM},        {0x12, DeviceType::DDR},
    {0x13, DeviceType::DDR2},         {0x14, DeviceType::DDR2_SDRAM_FB_DIMM},
    {0x18, DeviceType::DDR3},         {0x19, DeviceType::FBD2},
    {0x1a, DeviceType::DDR4},         {0x1b, DeviceType::LPDDR_SDRAM},
    {0x1c, DeviceType::LPDDR2_SDRAM}, {0x1d, DeviceType::LPDDR3_SDRAM},
    {0x1e, DeviceType::LPDDR4_SDRAM}, {0x1f, DeviceType::Logical},
    {0x20, DeviceType::HBM},          {0x21, DeviceType::HBM2},
    {0x22, DeviceType::DDR5},         {0x23, DeviceType::LPDDR5_SDRAM}};

const std::array<std::string, 16> detailTable{
    "Reserved",      "Other",         "Unknown",     "Fast-paged",
    "Static column", "Pseudo-static", "RAMBUS",      "Synchronous",
    "CMOS",          "EDO",           "Window DRAM", "Cache DRAM",
    "Non-volatile",  "Registered",    "Unbuffered",  "LRDIMM"};

/**
 * @brief Map SMBIOS 3.2 Memory Array Error Correction Types to
 * xyz.openbmc_project.Inventory.Item.Dimm.Ecc types.
 *
 * SMBIOS 3.2 Memory Array Error Correction Types 'Unknown', 'None', 'CRC' are
 * mapped to EccType::NoECC since the DBUs interface does not support those
 * representations.
 */
const std::map<uint8_t, EccType> dimmEccTypeMap = {
    {0x1, EccType::NoECC},        {0x2, EccType::NoECC},
    {0x3, EccType::NoECC},        {0x4, EccType::AddressParity},
    {0x5, EccType::SingleBitECC}, {0x6, EccType::MultiBitECC},
    {0x7, EccType::NoECC}};

#endif
} // namespace smbios

} // namespace phosphor
