#include "synapse.hpp"

#include <cmath>

Synapse::Synapse(std::size_t pre_id,
                 std::size_t post_id,
                 double weight,
                 double E_syn,
                 double tau_ms)
    : pre_id_(pre_id),
      post_id_(post_id),
      weight_(weight),
      E_syn_(E_syn),
      tau_ms_(tau_ms) {}

void Synapse::on_presynaptic_spike() {
  g_ += weight_;
}

void Synapse::decay(double dt) {
  if (tau_ms_ <= 0.0) {
    g_ = 0.0;
    return;
  }
  g_ *= std::exp(-dt / tau_ms_);
}

double Synapse::current(double V_post) const {
  return g_ * (V_post - E_syn_);
}
