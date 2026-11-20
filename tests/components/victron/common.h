#pragma once
#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "esphome/components/uart/uart_component.h"
#include "esphome/components/victron/victron.h"
#include "esphome/core/hal.h"

namespace esphome::victron::testing {

// Minimal UART that serves injected bytes to the component under test.
class MockUART : public uart::UARTComponent {
 public:
  void inject(const std::vector<uint8_t> &bytes) { this->rx_.insert(this->rx_.end(), bytes.begin(), bytes.end()); }
  size_t available() override { return this->rx_.size() - this->position_; }
  bool read_array(uint8_t *data, size_t len) override {
    if (len > this->available())
      return false;
    std::copy_n(this->rx_.data() + this->position_, len, data);
    this->position_ += len;
    return true;
  }
  bool peek_byte(uint8_t *data) override {
    if (this->available() == 0)
      return false;
    *data = this->rx_[this->position_];
    return true;
  }
  void write_array(const uint8_t *data, size_t len) override {}
  uart::UARTFlushResult flush() override { return uart::UARTFlushResult::UART_FLUSH_RESULT_ASSUMED_SUCCESS; }
  void check_logger_conflict() override {}
#if defined(USE_ESP8266) || defined(USE_ESP32)
  void load_settings(bool dump_config) override {}
#endif

 protected:
  std::vector<uint8_t> rx_;
  size_t position_{0};
};

class TestableVictron : public VictronComponent {
 public:
  TestableVictron() { this->set_uart_parent(&this->uart_); }

  using VictronComponent::clear_staged_;
  using VictronComponent::commit_staged_;
  using VictronComponent::handle_value_;
  using VictronComponent::label_;
  using VictronComponent::last_publish_;
  using VictronComponent::last_transmission_;
  using VictronComponent::value_;

  MockUART uart_;
};

struct VeDirectKV {
  const char *label;
  const char *value;
};

using Frame = std::vector<std::pair<std::string, std::string>>;

template<std::size_t N> Frame to_frame(const VeDirectKV (&kvs)[N]) {
  Frame frame;
  for (const auto &kv : kvs)
    frame.emplace_back(kv.label, kv.value);
  return frame;
}

// Encodes a VE.Direct text frame exactly as a device sends it: every field is
// prefixed with "\r\n", the frame ends with "\r\nChecksum\t" followed by a
// single byte that makes the modulo-256 sum of all frame bytes zero.
inline std::vector<uint8_t> encode_frame(const Frame &frame) {
  std::string raw;
  for (const auto &[label, value] : frame)
    raw += "\r\n" + label + "\t" + value;
  raw += "\r\nChecksum\t";
  uint8_t sum = 0;
  for (char c : raw)
    sum += static_cast<uint8_t>(c);
  std::vector<uint8_t> bytes(raw.begin(), raw.end());
  bytes.push_back(static_cast<uint8_t>(0x100 - sum));
  return bytes;
}

template<std::size_t N> std::vector<uint8_t> encode_frame(const VeDirectKV (&kvs)[N]) {
  return encode_frame(to_frame(kvs));
}

// Injects bytes and runs loop() until the UART is drained (loop() reads a
// bounded number of bytes per call).
inline void feed(TestableVictron &v, const std::vector<uint8_t> &bytes) {
  v.uart_.inject(bytes);
  while (v.uart_.available() > 0)
    v.loop();
}

// Lets the inter-byte timeout of loop() elapse without new data.
inline void expire_transmission_timeout(TestableVictron &v) {
  v.last_transmission_ = millis() - 1000;
  v.loop();
}

// Feeds a complete, valid frame through the UART byte stream.
template<std::size_t N> void process_frame(TestableVictron &v, const VeDirectKV (&frame)[N]) {
  feed(v, encode_frame(frame));
}

// Decodes a single field and publishes it immediately, independent of the
// checksum validation mode.
inline void decode(TestableVictron &v, const char *label, const char *value) {
  v.label_ = label;
  v.value_ = value;
  v.handle_value_();
  v.commit_staged_();
}

}  // namespace esphome::victron::testing
