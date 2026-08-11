#pragma once

#include "neuron.hpp"
#include "synapse.hpp"

#include <cstddef>
#include <memory>
#include <vector>

class Network {
 public:
  Neuron& add_neuron(std::string name, double C_m = 1.0, double V0 = -65.0);
  Synapse& add_synapse(std::size_t pre_id,
                       std::size_t post_id,
                       double weight,
                       double E_syn = 0.0,
                       double tau_ms = 2.0);

  std::size_t neuron_count() const { return neurons_.size(); }
  std::size_t synapse_count() const { return synapses_.size(); }

  Neuron& neuron(std::size_t id);
  const Neuron& neuron(std::size_t id) const;
  Synapse& synapse(std::size_t idx);
  const Synapse& synapse(std::size_t idx) const;

  // One network step: clear I_syn, apply synapses, integrate neurons, handle spikes.
  void step(double dt);

 private:
  std::vector<std::unique_ptr<Neuron>> neurons_;
  std::vector<std::unique_ptr<Synapse>> synapses_;
};
