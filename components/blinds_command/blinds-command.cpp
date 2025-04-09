#include "blinds-command.h"

namespace esphome {
namespace blinds_command {

static const char *TAG = "BlindsCommand";

// const char *BlindsCommand::cmds[BlindsCommand::CMDS_COUNT] = {"DOWN", "UP",    "STOP",
//                                                               "PAIR", "LIMIT", "CHGDIR"};

void BlindsCommand::setup() {}

void BlindsCommand::loop() {}

auto BlindsCommand::decode_cmd(std::string msg) -> bool {
  const char *ch;
  char cmd_str[10];
  int i;

  // Retrieve command

  ch = msg.c_str();
  i  = 0;
  while ((i < 9) && (*ch != 0) && (*ch != ',')) {
    if (*ch != ' ') cmd_str[i++] = *ch;
    ch++;
  }
  cmd_str[i] = 0;
  if (*ch != ',') {
    ESP_LOGE(TAG, "Command incomplete: %s", msg);
    return false;
  }

  i = 0;
  while (i < CMDS_COUNT) {
    if (strcmp(cmds[i], cmd_str) == 0) break;
    i++;
  }

  if (i >= CMDS_COUNT) {
    ESP_LOGE(TAG, "Unknown command: %s", cmd_str);
    return false;
  }
  cmd = (CMD)i;

  // Retrieve id

  ch++;
  while (*ch == ' ') ch++;
  id = 0;
  while ((*ch >= '0') && (*ch <= '9')) {
    id = (id * 10) + (*ch - '0');
    ch++;
  }
  while (*ch == ' ') ch++;

  ESP_LOGI(TAG, "Command: %s, Id: %d", cmd_str, id);

  // Retrieve channels

  if (*ch != ',') {
    ESP_LOGE(TAG, "No channel present: %s", msg.c_str());
    return false;
  }

  channel_count = 0;
  while ((*ch == ',') && (channel_count < 10)) {
    ch++;
    while (*ch == ' ') ch++;
    channels[channel_count] = 0;
    while ((*ch >= '0') && (*ch <= '9')) {
      channels[channel_count] = (channels[channel_count] * 10) + (*ch - '0');
      ch++;
    }
    while (*ch == ' ') ch++;
    channel_count++;
  }

  if (channel_count == 0) {
    ESP_LOGE(TAG, "No channel present: %s", msg.c_str());
    return false;
  } else {
    ESP_LOGI(TAG, "Channels Count: %d", channel_count);
  }

  while (*ch == ' ') ch++;

  if (*ch != 0) {
    ESP_LOGE(TAG, "Some residual data not processed: %s", msg.c_str());
    return false;
  }

#if DEBUGGING
  PRINTLN("Parsing command successfull:");
  PRINT("Cmd: ");
  PRINTLN(cmd_str);
  PRINT("Id: ");
  PRINTLN(id);
  PRINT("Channels:");

  char c = ' ';
  for (i = 0; i < channel_count; i++) {
    PRINT(c);
    PRINT(' ');
    PRINT(channels[i]);
    c = ',';
  }
  PRINTLN(".");
#endif

  return true;
}

void BlindsCommand::transmitHigh(int delay_microseconds) { pulses.push_back(delay_microseconds); }

void BlindsCommand::transmitLow(int delay_microseconds) { pulses.push_back(-delay_microseconds); }

void BlindsCommand::transmitValue(int value, int count) {
  int bit;
  while (count > 0) {
    bit = value & 1;
    value >>= 1;
    if (bit == 0) {
      transmitLow(PULSE_SHORT);
      transmitHigh(PULSE_SHORT);
      transmitLow(PULSE_SHORT);
    } else {
      transmitLow(PULSE_SHORT);
      transmitHigh(PULSE_LONG);
    }
    count--;
  }
}

auto BlindsCommand::emit_cmd() -> Pulses {
  unsigned char checksum;

  pulses.clear();
  pulses.reserve(200);

  for (int i = 0; i < channel_count; i++) {

    for (int j = 0; j < 10; j++) {

      transmitHigh(AGC1_PULSE);
      transmitLow(AGC2_PULSE);
      transmitHigh(AGC3_PULSE);

      checksum =
          ((id >> 8) & 0xFF) + (id & 0xFF) + ((cmd_bits[cmd] << 4) + channels[i]) + MODEL - 1;
      checksum = (checksum ^ 0xFF) + 1;

      transmitValue(id, 16);
      transmitValue(channels[i], 4);
      transmitValue(cmd_bits[cmd], 4);
      transmitValue(MODEL, 8);
      transmitValue(checksum, 8);
      transmitValue(1, 1);

      transmitLow(PULSE_SHORT);
    }

    // Radio silence at the end of last command.
    // It's better to go a bit over than under minimum required length:
    transmitLow(RADIO_SILENCE);
  }

  return pulses;
}

} // namespace blinds_command
} // namespace esphome