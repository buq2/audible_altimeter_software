#include "memory.hh"
#include "axlib/core/core.hh"

using namespace axlib;

MemoryController::MemoryController(FlashS25Fl216K *flash)
    :
      flash_(flash)
{
}

uint8_t MemoryController::WriteConfig(Config *config)
{
    // Config is stored in the first sector
    flash_->Erase4k(0);
    flash_->WriteData(0,(uint8_t*)config,sizeof(Config));
    return 0;
}

bool MemoryController::IsConfigValid()
{
    uint32_t magic;
    flash_->ReadData((uint8_t*)&magic,0,sizeof(magic));
    return magic == Config::GetValidMagic();
}

uint8_t MemoryController::LoadConfig(Config *config)
{
    if (!IsConfigValid()) {
        return 1;
    }
    flash_->ReadData((uint8_t*)config, 0, sizeof(Config));
    return 0;
}

uint32_t MemoryController::FindLastJumpSector()
{
    uint32_t sector = GetFirstPossibleJumpSector();
    while(IsJumpSector(sector)) {
        sector += GetSectorLength();
    }
    return sector-GetSectorLength();
}

uint32_t MemoryController::FindNextFreeSector()
{
    return FindLastJumpSector() + GetSectorLength();
}

bool MemoryController::IsJumpSector(uint32_t sector)
{
    sector = FlashS25Fl216K::GetSector4kBegin(sector);
    uint32_t magic;
    flash_->ReadData((uint8_t*)&magic,sector,sizeof(magic));
    return magic == JUMPDATA_HEADER_MAGIC;
}

uint32_t MemoryController::InitJumpSector()
{
    uint32_t sector = FindNextFreeSector();
    uint32_t prev_sector = sector-GetSectorLength();
    JumpData_Header header;
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

    return GetJumpNumberOnSector(sector);
}

uint32_t MemoryController::GetNumberOfJumps()
{
    uint32_t num_jumps = 0;
    uint32_t current_jump_number = -1;

    // Start from first sector which is available for jumps
    uint32_t sector = GetFirstPossibleJumpSector();
    while(IsJumpSector(sector)) {
        uint32_t jump_number = GetJumpNumberOnSector(sector);
        sector += GetSectorLength();

        if (jump_number != current_jump_number) {
            current_jump_number = jump_number;
            ++num_jumps;
        }
    }

    return num_jumps;
}

uint32_t MemoryController::GetJumpIndexSector(const uint32_t jump_idx)
{
    uint32_t current_jump_idx = 0;
    uint32_t current_jump_number = -1;

    // Start from first sector which is available for jumps
    uint32_t sector = GetFirstPossibleJumpSector();
    while(IsJumpSector(sector)) {
        uint32_t jump_number = GetJumpNumberOnSector(sector);

        if (jump_number != current_jump_number) {
            current_jump_number = jump_number;
            ++current_jump_idx;

            if (current_jump_idx == jump_idx) {
                return sector;
            }
        }

        sector += GetSectorLength();
    }

    // Not found
    return 0;
}

void MemoryController::GetJumpData(const uint32_t jump_idx, uint8_t *data, uint32_t *bytes, uint32_t *offset)
{
    uint32_t sector = GetJumpIndexSector(jump_idx);
    if (sector == 0) {
        // Not found
        *bytes = 0;
        return;
    }

    const uint32_t current_address = sector+*offset;
    const uint32_t jump_number = GetJumpNumberOnSector(sector);
    const uint32_t jump_number_current = GetJumpNumberOnSector(current_address);

    if (jump_number != jump_number_current) {
        // Error. At offset jump number has changed
        *bytes = 0;
        return;
    }

    const uint32_t next_page = FlashS25Fl216K::GetNext256Page(current_address);
    const uint32_t diff = next_page-current_address;
    const uint32_t bytes_to_read = axlib::min(diff,*bytes);

    flash_->ReadData(data, current_address, bytes_to_read);
    *bytes = bytes_to_read;
    *offset += *bytes;
}

uint32_t MemoryController::GetJumpNumberOnSector(uint32_t address)
{
    address = FlashS25Fl216K::GetSector4kBegin(address);

    JumpData_Header header;
    flash_->ReadData((uint8_t*)&header, address, sizeof(JumpData_Header));
    if (header.magic_number == JUMPDATA_HEADER_MAGIC) {
        // Valid header
        return header.jump_number;
    }

    // Failure
    return 0;
}

uint32_t MemoryController::GetFirstPossibleJumpSector()
{
    return 4096;
}

uint32_t MemoryController::GetSectorLength()
{
    return 4096;
}
