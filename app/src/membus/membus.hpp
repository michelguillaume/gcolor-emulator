//
// Created by Guillaume MICHEL on 15/01/2025.
//
// https://gbdev.io/pandocs/Memory_Map.html
//

#ifndef MEMBUS_HPP
#define MEMBUS_HPP

#include <cstdint>
#include <vector>

namespace emulator {
class Membus {
public:
  Membus() = default;
  ~Membus() = default;

  uint8_t read(uint16_t address) const;
  void write(uint16_t address, uint8_t value);
private:
  // Enum to represent the hardware model
  enum class Model {
    DMG,    // Original Game Boy
    MGB,    // Game Boy Pocket
    SGB,    // Super Game Boy
    SGB2,   // Super Game Boy 2
    CGB_0,  // Game Boy Color revision 0
    CGB_1,  // Game Boy Color revision 1
    CGB_A,  // Game Boy Color revision A
    CGB_B,  // Game Boy Color revision B
    CGB_C,  // Game Boy Color revision C
    CGB_D,  // Game Boy Color revision D
    CGB_E,  // Game Boy Color revision E (final revision)
    AGB,    // Game Boy Advance
    AGS,    // Game Boy Advance SP
    GBP     // Game Boy Player
};

  // Internal state
  std::array<uint8_t, 0x10000> memory{};   ///< Unified memory array
  uint8_t current_rom_bank = 1;            ///< Currently active ROM bank
  uint8_t dma_register = 0x00;             ///< DMA register
  bool oam_blocked = false;                ///< Whether OAM is blocked (e.g., during DMA)
  Model current_model = Model::DMG;        ///< Current Game Boy model (default: DMG)

  // Specific functions for certain memory regions
  uint8_t readROMBank1(uint16_t address) const;
  void writeROMBankControl(uint16_t address, uint8_t value);

  uint8_t readIORegisters(uint16_t address) const;
  void writeIORegisters(uint16_t address, uint8_t value);

  uint8_t readFEA0_FEFF(uint16_t address) const;
  static void writeFEA0_FEFF(uint16_t address, uint8_t value);

  static uint8_t getMaskedValueForCGB(uint16_t address);
};

}

#endif //MEMBUS_HPP
