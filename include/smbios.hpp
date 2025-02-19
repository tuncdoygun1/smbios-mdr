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

#include <phosphor-logging/elog-errors.hpp>

#include <array>
#define SMBIOS_MDRV1

#ifdef SMBIOS_MDRV1
#include <map>

static constexpr uint16_t mdrSMBIOSSize = 32 * 1024;    // 32K
static constexpr uint16_t mdrAcpiTableSize = 32 * 1024; // 32K
static constexpr uint16_t mdrMemMappingSize = 8 * 1024; // 8K
static constexpr uint16_t mdrScsiBootSize = 8 * 1024;   // 8K
static constexpr uint16_t mdrNvmeSize = 1 * 1024;       // 1K

static constexpr uint16_t smbiosTableStorageSize = 0xffff;

static constexpr uint8_t mdrVersion = 0x11; // MDR version 1.1

static constexpr const char *smbiosPath = "/etc/smbios";
static constexpr const char *mdrType1File = "/etc/smbios/smbios1";
static constexpr const char *mdrType2File = "/etc/smbios/smbios2";
static constexpr const char *mdrAcpiFile = "/etc/smbios/acpi";
static constexpr const char *mdrMemMapFile = "/etc/smbios/memmapping";
static constexpr const char *mdrScsiBootFile = "/etc/smbios/scsiboot";
static constexpr const char *mdrNvmeFile = "/etc/smbios/nvme";

typedef enum
{
    mdrNone = 0,
    mdrSmbios = 1,
    mdrAcpi = 2,
    mdrMemMap = 3,
    mdrScsiBoot = 4,
    mdrNvme = 5,
    maxMdrIndex = 6
} MDRRegionIndex;

typedef enum
{
    regionLockUnlocked = 0,
    regionLockStrict,
    regionLockPreemptable
} MDRLockType;

struct MDRState
{
    uint8_t mdrVersion;
    uint8_t regionId;
    uint8_t valid;
    uint8_t updateCount;
    uint8_t lockPolicy;
    uint16_t regionLength;
    uint16_t regionUsed;
    uint8_t crc8;
} __attribute__((packed));

struct ManagedDataRegion
{
    const char *flashName;
    uint8_t *regionData;
    uint16_t msTimeout;
    struct MDRState state;
    uint8_t sessionId;
} __attribute__((packed));

struct BIOSInfo
{
    uint8_t biosVersion;
};

#elifdef SMBIOS_MDRV2

static constexpr const char* mdrType2File = "/var/lib/smbios/smbios2";
static constexpr const char* smbiosPath = "/var/lib/smbios";

static constexpr uint16_t mdrSMBIOSSize = 32 * 1024;

constexpr uint16_t smbiosAgentId = 0x0101;
constexpr int firstAgentIndex = 1;

constexpr uint8_t maxDirEntries = 4;
constexpr uint32_t mdr2SMSize = 0x00100000;
constexpr uint32_t mdr2SMBaseAddress = 0x9FF00000;

constexpr uint8_t mdrTypeII = 2;

constexpr uint8_t mdr2Version = 2;
constexpr uint8_t smbiosAgentVersion = 1;

constexpr uint32_t pageMask = 0xf000;
constexpr int smbiosDirIndex = 0;

constexpr uint32_t smbiosTableVersion = 15;
constexpr uint32_t smbiosTableTimestamp = 0x45464748;
constexpr uint32_t smbiosSMMemoryOffset = 0;
constexpr uint32_t smbiosSMMemorySize = 1024 * 1024;
constexpr uint32_t smbiosTableStorageSize = 64 * 1024;
constexpr uint32_t defaultTimeout = 20000;

enum class MDR2SMBIOSStatusEnum
{
    mdr2Init = 0,
    mdr2Loaded = 1,
    mdr2Updated = 2,
    mdr2Updating = 3
};

enum class MDR2DirLockEnum
{
    mdr2DirUnlock = 0,
    mdr2DirLock = 1
};

enum class DirDataRequestEnum
{
    dirDataNotRequested = 0x00,
    dirDataRequested = 0x01
};

enum class FlagStatus
{
    flagIsInvalid = 0,
    flagIsValid = 1,
    flagIsLocked = 2
};

typedef struct
{
    uint8_t dataInfo[16];
} DataIdStruct;

typedef struct
{
    DataIdStruct id;
    uint32_t size;
    uint32_t dataSetSize;
    uint8_t dataVersion;
    uint32_t timestamp;
} Mdr2DirEntry;

typedef struct
{
    Mdr2DirEntry common;
    MDR2SMBIOSStatusEnum stage;
    MDR2DirLockEnum lock;
    uint16_t lockHandle;
    uint32_t xferBuff;
    uint32_t xferSize;
    uint32_t maxDataSize;
    uint8_t* dataStorage;
} Mdr2DirLocalStruct;

typedef struct
{
    uint8_t agentVersion;
    uint8_t dirVersion;
    uint8_t dirEntries;
    uint8_t status; // valid / locked / etc
    uint8_t remoteDirVersion;
    uint16_t sessionHandle;
    Mdr2DirLocalStruct dir[maxDirEntries];
} Mdr2DirStruct;

struct MDRSMBIOSHeader
{
    uint8_t dirVer;
    uint8_t mdrType;
    uint32_t timestamp;
    uint32_t dataSize;
} __attribute__((packed));

typedef struct
{
    uint8_t majorVersion;
    uint8_t minorVersion;
} SMBIOSVersion;

struct EntryPointStructure21
{
    uint32_t anchorString;
    uint8_t epChecksum;
    uint8_t epLength;
    SMBIOSVersion smbiosVersion;
    uint16_t maxStructSize;
    uint8_t epRevision;
    uint8_t formattedArea[5];
    uint8_t intermediateAnchorString[5];
    uint8_t intermediateChecksum;
    uint16_t structTableLength;
    uint32_t structTableAddress;
    uint16_t noOfSmbiosStruct;
    uint8_t smbiosBDCRevision;
} __attribute__((packed));

struct EntryPointStructure30
{
    uint8_t anchorString[5];
    uint8_t epChecksum;
    uint8_t epLength;
    SMBIOSVersion smbiosVersion;
    uint8_t smbiosDocRev;
    uint8_t epRevision;
    uint8_t reserved;
    uint32_t structTableMaxSize;
    uint64_t structTableAddr;
} __attribute__((packed));

constexpr std::array<SMBIOSVersion, 3> supportedSMBIOSVersions{
    SMBIOSVersion{3, 2}, SMBIOSVersion{3, 3}, SMBIOSVersion{3, 5}};
    
#endif


static constexpr const char* cpuPath =
    "/xyz/openbmc_project/inventory/system/chassis/motherboard/cpu";

static constexpr const char* dimmPath =
    "/xyz/openbmc_project/inventory/system/chassis/motherboard/dimm";

static constexpr const char* pciePath =
    "/xyz/openbmc_project/inventory/system/chassis/motherboard/pcieslot";

static constexpr const char* systemPath =
    "/xyz/openbmc_project/inventory/system/chassis/motherboard/bios";

typedef enum
{
    biosType = 0,
    systemType = 1,
    baseboardType = 2,
    chassisType = 3,
    processorsType = 4,
    memoryControllerType = 5,
    memoryModuleInformationType = 6,
    cacheType = 7,
    portConnectorType = 8,
    systemSlots = 9,
    onBoardDevicesType = 10,
    oemStringsType = 11,
    systemCconfigurationOptionsType = 12,
    biosLanguageType = 13,
    groupAssociatonsType = 14,
    systemEventLogType = 15,
    physicalMemoryArrayType = 16,
    memoryDeviceType = 17,
} SmbiosType;

static constexpr uint8_t separateLen = 2;

static inline uint8_t* smbiosNextPtr(uint8_t* smbiosDataIn)
{
    if (smbiosDataIn == nullptr)
    {
        return nullptr;
    }
    uint8_t* smbiosData = smbiosDataIn + *(smbiosDataIn + 1);
    int len = 0;
    while ((*smbiosData | *(smbiosData + 1)) != 0)
    {
        smbiosData++;
        len++;
        if (len >= mdrSMBIOSSize) // To avoid endless loop
        {
            return nullptr;
        }
    }
    return smbiosData + separateLen;
}

// When first time run getSMBIOSTypePtr, need to send the RegionS[].regionData
// to smbiosDataIn
static inline uint8_t* getSMBIOSTypePtr(uint8_t* smbiosDataIn, uint8_t typeId,
                                        size_t size = 0)
{
    if (smbiosDataIn == nullptr)
    {
        return nullptr;
    }
    char* smbiosData = reinterpret_cast<char*>(smbiosDataIn);
    while ((*smbiosData != '\0') || (*(smbiosData + 1) != '\0'))
    {
        uint32_t len = *(smbiosData + 1);
        if (*smbiosData != typeId)
        {
#ifdef SMBIOS_MDRV1
			uint32_t len = *(smbiosData + 1);
#endif			
            smbiosData += len;
            while ((*smbiosData != '\0') || (*(smbiosData + 1) != '\0'))
            {
                smbiosData++;
                len++;
                if (len >= mdrSMBIOSSize) // To avoid endless loop
                {
                    return nullptr;
                }
            }
            smbiosData += separateLen;
            continue;
        }
#ifdef SMBIOS_MDRV2        
        if (len < size)
        {
            phosphor::logging::log<phosphor::logging::level::ERR>(
                "Record size mismatch!");
            return nullptr;
        }
#endif        
        return reinterpret_cast<uint8_t*>(smbiosData);
    }
    return nullptr;
}

static inline std::string positionToString(uint8_t positionNum,
                                           uint8_t structLen, uint8_t* dataIn)
{
#ifdef SMBIOS_MDRV1
	if (dataIn == nullptr)
#elifdef SMBIOS_MDRV2
    if (dataIn == nullptr || positionNum == 0)
#endif    
    {
        return "";
    }
    uint16_t limit = mdrSMBIOSSize; // set a limit to avoid endless loop

    char* target = reinterpret_cast<char*>(dataIn + structLen);
    if (target == nullptr)
    {
        return "";
    }
    for (uint8_t index = 1; index < positionNum; index++)
    {
        for (; *target != '\0'; target++)
        {
            limit--;
            // When target = dataIn + structLen + limit,
            // following target++ will be nullptr
#ifdef SMBIOS_MDRV1
			if (limit < 1)
#elifdef SMBIOS_MDRV2
            if (limit < 1 || target == nullptr)
#endif
            {
                return "";
            }
        }
        target++;
#ifdef SMBIOS_MDRV1
		if (*target == '\0')
#elifdef SMBIOS_MDRV2   
        if (target == nullptr || *target == '\0')
#endif
        {
            return ""; // 0x00 0x00 means end of the entry.
        }
    }

    std::string result = target;
    return result;
}

#ifdef SMBIOS_MDRV1
// Find the specific string in smbios item
static inline std::string seekString(uint8_t *smbiosDataIn, uint8_t stringOrder)
{
    if (smbiosDataIn == nullptr)
        return "";
    uint8_t len = *(smbiosDataIn + 1);

    return positionToString(stringOrder, len, smbiosDataIn);
}
#endif