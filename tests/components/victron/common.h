#pragma once
#include "esphome/components/victron/victron.h"

namespace esphome::victron::testing {

class TestableVictron : public VictronComponent {
 public:
  using VictronComponent::handle_value_;
  using VictronComponent::label_;
  using VictronComponent::value_;
};

struct VeDirectKV {
  const char *label;
  const char *value;
};

template<std::size_t N> void process_frame(TestableVictron &v, const VeDirectKV (&frame)[N]) {
  for (const auto &kv : frame) {
    v.label_ = kv.label;
    v.value_ = kv.value;
    v.handle_value_();
  }
}

}  // namespace esphome::victron::testing
