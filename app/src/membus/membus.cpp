//
// Created by Guillaume MICHEL on 15/01/2025.
//

#include "membus.hpp"

#include <iostream>
#include <ostream>
#include <stdexcept>

namespace emulator {

// Memory ranges defined with constexpr
constexpr uint16_t ROM_BANK_0_START = 0x0000;
constexpr uint16_t ROM_BANK_0_END = 0x3FFF;

constexpr uint16_t ROM_BANK_1_START = 0x4000;
constexpr uint16_t ROM_BANK_1_END = 0x7FFF;

constexpr uint16_t VRAM_START = 0x8000;
constexpr uint16_t VRAM_END = 0x9FFF;

constexpr uint16_t EXTERNAL_RAM_START = 0xA000;
constexpr uint16_t EXTERNAL_RAM_END = 0xBFFF;

constexpr uint16_t WRAM_0_START = 0xC000;
constexpr uint16_t WRAM_0_END = 0xCFFF;

constexpr uint16_t WRAM_1_START = 0xD000;
constexpr uint16_t WRAM_1_END = 0xDFFF;

constexpr uint16_t ECHO_RAM_START = 0xE000;
constexpr uint16_t ECHO_RAM_END = 0xFDFF;

constexpr uint16_t OAM_START = 0xFE00;
constexpr uint16_t OAM_END = 0xFE9F;

constexpr uint16_t IO_REGISTERS_START = 0xFF00;
constexpr uint16_t IO_REGISTERS_END = 0xFF7F;

constexpr uint16_t HRAM_START = 0xFF80;
constexpr uint16_t HRAM_END = 0xFFFE;

constexpr uint16_t INTERRUPT_ENABLE = 0xFFFF;

// Initialize Echo RAM to mirror Work RAM in constructor for optimisation
//for (uint16_t addr = 0xE000; addr <= 0xFDFF; ++addr) {
//  memory[addr] = memory[addr - 0x2000];
//}

uint8_t Membus::read(const uint16_t address) const {
  switch (address) {
  case ROM_BANK_0_START ... ROM_BANK_0_END:
    return memory[address];
  case ROM_BANK_1_START ... ROM_BANK_1_END:
    return readROMBank1(address);
  case VRAM_START ... VRAM_END:
    return memory[address];  // Specific logic for CGB can be added
  case EXTERNAL_RAM_START ... EXTERNAL_RAM_END:
    return memory[address];
  case WRAM_0_START ... WRAM_0_END:
    return memory[address];
  case WRAM_1_START ... WRAM_1_END:
    return memory[address];
  case ECHO_RAM_START ... ECHO_RAM_END:
    return memory[address - 0x2000];  // Handle mirroring
  case OAM_START ... OAM_END:
    return memory[address];
  case 0xFEA0 ... 0xFEFF:  // Special range
    return readFEA0_FEFF(address);
  case IO_REGISTERS_START ... IO_REGISTERS_END:
    return readIORegisters(address);
  case HRAM_START ... HRAM_END:
    return memory[address];
  case INTERRUPT_ENABLE:
    return memory[address];
  default:
    return 0xFF;  // Unmapped memory
  }
}

void Membus::write(uint16_t address, uint8_t value) {
  switch (address) {
  case ROM_BANK_0_START ... ROM_BANK_0_END:
    // ROM Bank 0 is read-only
      break;
  case ROM_BANK_1_START ... ROM_BANK_1_END:
    writeROMBankControl(address, value);
    break;
  case VRAM_START ... VRAM_END:
    memory[address] = value;  // Specific logic for CGB can be added
    break;
  case EXTERNAL_RAM_START ... EXTERNAL_RAM_END:
    memory[address] = value;
    break;
  case WRAM_0_START ... WRAM_0_END:
    memory[address] = value;
    break;
  case WRAM_1_START ... WRAM_1_END:
    memory[address] = value;
    break;
  case ECHO_RAM_START ... ECHO_RAM_END:
    memory[address - 0x2000] = value;  // Handle mirroring
    break;
  case OAM_START ... OAM_END:
    memory[address] = value;
    break;
  case 0xFEA0 ... 0xFEFF:  // Special range
    return writeFEA0_FEFF(address, value);
  case IO_REGISTERS_START ... IO_REGISTERS_END:
    writeIORegisters(address, value);
    break;
  case HRAM_START ... HRAM_END:
    memory[address] = value;
    break;
  case INTERRUPT_ENABLE:
    memory[address] = value;
    break;
  default:
    break;  // Unmapped memory
  }
}

uint8_t Membus::readROMBank1(const uint16_t address) const {
  const uint16_t relative_address = address - ROM_BANK_1_START;
  //const uint16_t banked_address = (current_rom_bank * 0x4000) + relative_address;

  // Compute the banked address using bitwise shift for performance
  const uint32_t banked_address = (static_cast<uint32_t>(current_rom_bank) << 14) + relative_address;

  if (banked_address >= memory.size()) {
    std::cerr << "Error: Invalid banked address " << banked_address
              << " for ROM bank " << current_rom_bank << std::endl;
    return 0xFF;
  }

  return memory[banked_address];
}

void Membus::writeROMBankControl(const uint16_t address, const uint8_t value) {
  if (address >= 0x2000 && address < 0x4000) {
    current_rom_bank = value & 0x1F;  // Limit to 5 bits
  }
}

uint8_t Membus::readIORegisters(const uint16_t address) const {
  if (address == 0xFF46) {  // Example: DMA register
    return dma_register;
  }
  return memory[address];
}

void Membus::writeIORegisters(const uint16_t address, const uint8_t value) {
  if (address == 0xFF46) {  // DMA
    dma_register = value;
    // Handle DMA logic here
  } else {
    memory[address] = value;
  }
}

uint8_t Membus::readFEA0_FEFF(const uint16_t address) const {
  // Handle OAM blocked state
  if (oam_blocked) {
    return 0xFF;  // Return 0xFF when OAM is blocked
  }

  switch (current_model) {
  case Model::DMG ... Model::SGB2:  // Older models (DMG, MGB, SGB, SGB2)
#warning need to handle OAM Corruption Bug see -> https://gbdev.io/pandocs/OAM_Corruption_Bug.html#oam-corruption-bug
    return 0x00;  // Return 0x00 if not blocked

  case Model::CGB_0 ... Model::CGB_D:  // CGB revisions 0-D
    // Masked value for these CGB revisions
      return getMaskedValueForCGB(address);

  case Model::CGB_E ... Model::GBP:  // CGB revision E and later models
    // Return high nibble mirrored
      return ((address & 0xF0) >> 4) | ((address & 0xF0) & 0xF0);

  default:
    return 0xFF;  // Default for unsupported models
  }
}


void Membus::writeFEA0_FEFF(const uint16_t address, const uint8_t value) {
  // Writes to this area are prohibited, ignore them
  // Optionally log or handle as a no-op
}

uint8_t Membus::getMaskedValueForCGB(const uint16_t address) {
  // Hypothetical mask: use the lower 6 bits of the address
  return address & 0x3F;
}

}
