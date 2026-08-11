#include "neuron.hpp"

Neuron::Neuron(std::string name, double C_m, double V0)
    : name_(std::move(name)), C_m_(C_m), V_(V0) {
  na_.set_steady_state(V_);
  k_.set_steady_state(V_);
  above_threshold_ = V_ >= kSpikeThreshold;
}

NeuronState Neuron::snapshot() const {
  NeuronState s;
  s.V = V_;
  s.m = na_.m();
  s.h = na_.h();
  s.n = k_.n();
  s.I_Na = na_.current(V_);
  s.I_K = k_.current(V_);
  s.I_L = leak_.current(V_);
  s.I_syn = I_syn_;
  s.I_ext = I_ext_;
  s.spiked = spiked_;
  return s;
}

void Neuron::step(double dt) {
  spiked_ = false;

  const double I_Na = na_.current(V_);
  const double I_K = k_.current(V_);
  const double I_L = leak_.current(V_);
  const double I_total = I_ext_ + I_syn_ - I_Na - I_K - I_L;

  V_ += dt * I_total / C_m_;

  na_.step(dt, V_);
  k_.step(dt, V_);

  const bool now_above = V_ >= kSpikeThreshold;
  if (now_above && !above_threshold_) {
    spiked_ = true;
  }
  above_threshold_ = now_above;
}
