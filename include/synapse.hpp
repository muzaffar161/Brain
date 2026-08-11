#pragma once

#include <cstddef>

class Neuron;

// Conductance-based chemical synapse:
//   I_syn = g * (V_post - E_syn)
//   dg/dt = -g / tau
// On presynaptic spike: g += weight
class Synapse {
 public:
  Synapse(std::size_t pre_id,
          std::size_t post_id,
          double weight,
          double E_syn = 0.0,
          double tau_ms = 2.0);

  std::size_t pre_id() const { return pre_id_; }
  std::size_t post_id() const { return post_id_; }
  double g() const { return g_; }
  double weight() const { return weight_; }
  double E_syn() const { return E_syn_; }

  void on_presynaptic_spike();
  void decay(double dt);
  double current(double V_post) const;

 private:
  std::size_t pre_id_;
  std::size_t post_id_;
  double weight_;
  double E_syn_;
  double tau_ms_;
  double g_ = 0.0;
};
