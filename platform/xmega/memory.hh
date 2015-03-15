#ifndef AUDIBLEA_MEMORY_HH
#define AUDIBLEA_MEMORY_HH

#include "jump_data.hh"
#include "axlib/memory/flash_s25fl216k.hh"

class MemoryController
{
 public:
    MemoryController(axlib::FlashS25Fl216K *flash);

    /// \return 0 on success
    uint8_t WriteConfig(Config *config);
    bool IsConfigValid();

    /// \return 0 on success
    uint8_t LoadConfig(Config *config);
    uint32_t FindLastJumpSector();
    uint32_t FindNextFreeSector();
    bool IsJumpSector(uint32_t sector);

    /// Returns pointer to memory address to which jump data can be written
    /// Beginning of the sector is initialized with JumpData_Header
    uint32_t InitJumpSector();

    /// Get last jump number
    uint32_t GetLastJumpNumber();

    /// Get number of jumps
    uint32_t GetNumberOfJumps();

    uint32_t GetJumpIndexSector(const uint32_t jump_idx);

    /// Get jump data for certain jump index
    /// \param[in] jump_idx From 0 to GetNumberOfJumps-1. Not same as jump number
    /// \param[in,out] bytes As input buffer length. As output number of bytes read.
    ///     If 0, no more jump data for this jump.
    /// \param[in,out] offset From which offset the data should be read. As output, next unread byte
    void GetJumpData(const uint32_t jump_idx, uint8_t *data, uint32_t *bytes, uint32_t *offset);

    uint32_t GetJumpNumberOnSector(uint32_t address);

    static uint32_t GetFirstPossibleJumpSector();
    static uint32_t GetSectorLength();
 private:
    axlib::FlashS25Fl216K *flash_;
}; // class MemoryController

#endif //ifndef AUDIBLEA_MEMORY_HH
