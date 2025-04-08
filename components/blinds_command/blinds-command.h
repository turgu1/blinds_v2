#pragma once

#ifndef DEBUGGING
#define DEBUGGING 0
#endif

#include <cstring>
#include <string>
#include <tuple>
#include <vector>

#include "esphome.h"
#include "esphome/core/component.h"

namespace esphome {
namespace blinds_command {

class BlindsCommand : public Component {
private:
  bool initialized = false;

  using Pulses = std::vector<long int>;

  static constexpr int CMDS_COUNT = 6;

  enum CMD { DOWN = 0, UP, STOP, PAIR, LIMIT, CHGDIR };

  static constexpr const char *cmds[CMDS_COUNT] = {"DOWN", "UP", "STOP", "PAIR", "LIMIT", "CHGDIR"};

  static constexpr char cmd_bits[CMDS_COUNT] = {1, 12, 5, 4, 2, 8};

  static const int PULSE_SHORT = 340;
  static const int PULSE_LONG  = 680;

  static const int AGC1_PULSE = 4885;
  static const int AGC2_PULSE = 2450;
  static const int AGC3_PULSE = 1700;

  static const int RADIO_SILENCE = 5057;
  static const int MODEL         = 0xC1;

  CMD cmd;
  int id;
  int channel_count;
  short channels[10];

  Pulses pulses;

  void transmitHigh(int delay_microseconds);
  void transmitLow(int delay_microseconds);
  void transmitValue(int value, int count);

public:
  void setup() override;
  void loop() override;

  auto decode_cmd(std::string msg) -> bool;
  auto emit_cmd() -> Pulses;
};

} // namespace blinds_command
} // namespace esphome
