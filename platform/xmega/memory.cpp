#include "memory.hh"

using namespace axlib;

MemoryController::MemoryController(FlashS25Fl216K *flash)
    :
      flash_(flash)
{
}

void MemoryController::WriteConfig(Config *config)
{
    // Config is stored in the first sector
    flash_->Erase4k(0);
    flash_->WriteData(0,(uint8_t*)config,sizeof(Config));
}

bool MemoryController::IsConfigValid()
{
    uint32_t magic;
    flash_->ReadData((uint8_t*)&magic,0,sizeof(magic));
    return magic == Config::GetValidMagic();
}

uint32_t MemoryController::FindLastJumpSector()
{
    uint32_t sector = 4096;
    while(IsJumpSector(sector)) {
        sector += 4096;
    }
    return sector-4096;
}

uint32_t MemoryController::FindNextFreeSector()
{
    return FindLastJumpSector() + 4096;
}

bool MemoryController::IsJumpSector(uint32_t sector)
{
    sector &= 0xfffff000;
    uint32_t magic;
    flash_->ReadData((uint8_t*)&magic,sector,sizeof(magic));
    return magic == JUMPDATA_HEADER_MAGIC;
}

uint32_t MemoryController::InitJumpSector()
{
    uint32_t sector = FindNextFreeSector();
    uint32_t prev_sector = sector-4096;
    JumpData_Header header;
    header.magic_number = JUMPDATA_HEADER_MAGIC;
    if (IsJumpSector(prev_sector)) {
        JumpData_Header prev_header;
        flash_->ReadData((uint8_t*)&prev_header, prev_sector, sizeof(JumpData_Header));
        header.jump_number = prev_header.jump_number + 1;
    } else {
        header.jump_number = 1;
    }

    flash_->Erase4k(sector);
    flash_->WriteData(sector,(uint8_t*)&header,sizeof(header));

    return sector + sizeof(header);
}

uint32_t MemoryController::GetLastJumpNumber()
{
    uint32_t sector = FindLastJumpSector();
    if (sector == 0) {
        // No jumps
        return 0;
    }

    JumpData_Header header;
    flash_->ReadData((uint8_t*)&header, sector, sizeof(JumpData_Header));
    if (header.magic_number == JUMPDATA_HEADER_MAGIC) {
        // Valid header
        return header.jump_number;
    }

    // Failure
    return 0;
}
