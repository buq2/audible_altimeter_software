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

    /// \return Memory address of last jump sector
    uint32_t FindLastJumpSector();

    /// \return Memory address of next unused sector
    uint32_t FindNextFreeSector();

    /// \param[in] Address in memory which should be checked if it belongs to
    ///     jump data
    /// \return True if given sector is jump sector
    bool IsJumpSector(uint32_t sector);

    /// Returns pointer to memory address to which jump data can be written
    /// Beginning of the sector is initialized with JumpData_Header
    uint32_t InitJumpSector_ForNewJump();

    /// Initialize jump sector in memory address
    /// \param[in] sector Memory address which should be initialized as jump
    ///     data beginning
    uint32_t InitJumpSector(const uint32_t sector, const uint32_t jump_number);

    /// Get last jump number
    uint32_t GetLastJumpNumber();

    /// Quick erase the memory (destroy beginning of the flash chip)
    void QuickErase();

    /// Fully erase the flash chip
    void FullErase();

    /// Get number of jumps in the flash choå
    uint32_t GetNumberOfJumps();

    /// \param[in] jump_idx Jump idx number (0 to GetNumberOfJumps()-1)
    /// \return Memory address from which the jump data begins or 0 if not found
    uint32_t GetJumpIndexSector(const uint32_t jump_idx);

    /// Get jump data for certain jump index
    /// \param[in] jump_idx From 0 to GetNumberOfJumps-1. Not same as jump number
    /// \param[in,out] bytes As input buffer length. As output number of bytes read.
    ///     If 0, no more jump data for this jump.
    /// \param[in,out] offset From which offset the data should be read. As output, next unread byte
    void GetJumpData(const uint32_t jump_idx, uint8_t *data, uint32_t *bytes, uint32_t *offset);

    /// \return Jump number of
    uint32_t GetJumpNumberOnSector(uint32_t address);

    /// Write jump data and increment address
    /// Handles sector boundaries properly
    void WriteJumpData(const uint8_t *data, const uint8_t bytes, uint32_t *address);

    /// \return First memory address which may contain jump data.
    static uint32_t GetFirstPossibleJumpSector();

    /// \return Length of the sector in bytes
    static uint32_t GetSectorLength();

    /// \return Flash chip controller
    axlib::FlashS25Fl216K *GetFlash();
 private:
    axlib::FlashS25Fl216K *flash_;
    uint32_t current_jump_number_;
}; // class MemoryController

#endif //ifndef AUDIBLEA_MEMORY_HH
