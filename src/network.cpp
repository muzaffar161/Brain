#include "network.hpp"

#include <stdexcept>
#include <utility>

Neuron& Network::add_neuron(std::string name, double C_m, double V0) {
  auto neuron = std::make_unique<Neuron>(std::move(name), C_m, V0);
  neuron->set_id(neurons_.size());
  neurons_.push_back(std::move(neuron));
  return *neurons_.back();
}

Synapse& Network::add_synapse(std::size_t pre_id,
                              std::size_t post_id,
                              double weight,
                              double E_syn,
                              double tau_ms) {
  if (pre_id >= neurons_.size() || post_id >= neurons_.size()) {
    throw std::out_of_range("synapse neuron id out of range");
  }
  synapses_.push_back(
      std::make_unique<Synapse>(pre_id, post_id, weight, E_syn, tau_ms));
  return *synapses_.back();
}

Neuron& Network::neuron(std::size_t id) {
  return *neurons_.at(id);
}

const Neuron& Network::neuron(std::size_t id) const {
  return *neurons_.at(id);
}

Synapse& Network::synapse(std::size_t idx) {
  return *synapses_.at(idx);
}

const Synapse& Network::synapse(std::size_t idx) const {
  return *synapses_.at(idx);
}

void Network::step(double dt) {
  for (auto& n : neurons_) {
    n->clear_I_syn();
  }

  for (auto& syn : synapses_) {
    syn->decay(dt);
    Neuron& post = *neurons_.at(syn->post_id());
    // Convention: I_syn added to neuron is outward-negative ionic form
    // already handled in Neuron::step as +I_syn. Conductance current
    // g*(V-E) is outward when V > E; neuron equation uses -ΣI_ionic + I_ext + I_syn,
    // so we inject -I_syn_conductance so positive drive depolarizes when E_syn > V.
    post.add_I_syn(-syn->current(post.V()));
  }

  for (auto& n : neurons_) {
    n->step(dt);
  }

  for (auto& syn : synapses_) {
    if (neurons_.at(syn->pre_id())->spiked()) {
      syn->on_presynaptic_spike();
    }
  }
}
