#ifndef AUDIBLEA_MEMORY_HH
#define AUDIBLEA_MEMORY_HH

#include "jump_data.hh"
#include "axlib/memory/flash_s25fl216k.hh"

class MemoryController
{
 public:
    MemoryController(axlib::FlashS25Fl216K *flash);
    void WriteConfig(Config *config);
    bool IsConfigValid();
    uint32_t FindLastJumpSector();
    uint32_t FindNextFreeSector();
    bool IsJumpSector(uint32_t sector);

    /// Returns pointer to memory address to which jump data can be written
    /// Beginning of the sector is initialized with JumpData_Header
    uint32_t InitJumpSector();

    /// Get last jump number
    uint32_t GetLastJumpNumber();
 private:
    axlib::FlashS25Fl216K *flash_;
}; // class MemoryController

#endif //ifndef AUDIBLEA_MEMORY_HH
