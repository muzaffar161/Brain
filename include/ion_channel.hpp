#pragma once

#include <cmath>
#include <string>

// Hodgkin–Huxley ion channels (physiological voltage convention, mV / ms).
// Currents in µA/cm², conductances in mS/cm².

inline double hh_alpha_m(double V) {
  const double x = V + 40.0;
  if (std::abs(x) < 1e-6) {
    return 1.0;
  }
  return 0.1 * x / (1.0 - std::exp(-x / 10.0));
}

inline double hh_beta_m(double V) {
  return 4.0 * std::exp(-(V + 65.0) / 18.0);
}

inline double hh_alpha_h(double V) {
  return 0.07 * std::exp(-(V + 65.0) / 20.0);
}

inline double hh_beta_h(double V) {
  return 1.0 / (1.0 + std::exp(-(V + 35.0) / 10.0));
}

inline double hh_alpha_n(double V) {
  const double x = V + 55.0;
  if (std::abs(x) < 1e-6) {
    return 0.1;
  }
  return 0.01 * x / (1.0 - std::exp(-x / 10.0));
}

inline double hh_beta_n(double V) {
  return 0.125 * std::exp(-(V + 65.0) / 80.0);
}

inline double hh_gate_inf(double alpha, double beta) {
  return alpha / (alpha + beta);
}

inline double hh_gate_tau(double alpha, double beta) {
  return 1.0 / (alpha + beta);
}

// Advance a gating variable with forward Euler: dx/dt = (x_inf - x) / tau
inline double step_gate(double x, double alpha, double beta, double dt) {
  const double x_inf = hh_gate_inf(alpha, beta);
  const double tau = hh_gate_tau(alpha, beta);
  return x + dt * (x_inf - x) / tau;
}

class IonChannel {
 public:
  virtual ~IonChannel() = default;
  virtual const char* name() const = 0;
  virtual double conductance() const = 0;
  virtual double current(double V) const = 0;
  virtual void step(double dt, double V) = 0;
  virtual void set_steady_state(double V) = 0;
};

class SodiumChannel final : public IonChannel {
 public:
  explicit SodiumChannel(double g_max = 120.0, double E = 50.0)
      : g_max_(g_max), E_(E), m_(0.0), h_(0.0) {}

  const char* name() const override { return "Na"; }

  double conductance() const override {
    return g_max_ * m_ * m_ * m_ * h_;
  }

  double current(double V) const override {
    return conductance() * (V - E_);
  }

  void step(double dt, double V) override {
    m_ = step_gate(m_, hh_alpha_m(V), hh_beta_m(V), dt);
    h_ = step_gate(h_, hh_alpha_h(V), hh_beta_h(V), dt);
  }

  void set_steady_state(double V) override {
    m_ = hh_gate_inf(hh_alpha_m(V), hh_beta_m(V));
    h_ = hh_gate_inf(hh_alpha_h(V), hh_beta_h(V));
  }

  double m() const { return m_; }
  double h() const { return h_; }

 private:
  double g_max_;
  double E_;
  double m_;
  double h_;
};

class PotassiumChannel final : public IonChannel {
 public:
  explicit PotassiumChannel(double g_max = 36.0, double E = -77.0)
      : g_max_(g_max), E_(E), n_(0.0) {}

  const char* name() const override { return "K"; }

  double conductance() const override {
    return g_max_ * n_ * n_ * n_ * n_;
  }

  double current(double V) const override {
    return conductance() * (V - E_);
  }

  void step(double dt, double V) override {
    n_ = step_gate(n_, hh_alpha_n(V), hh_beta_n(V), dt);
  }

  void set_steady_state(double V) override {
    n_ = hh_gate_inf(hh_alpha_n(V), hh_beta_n(V));
  }

  double n() const { return n_; }

 private:
  double g_max_;
  double E_;
  double n_;
};

class LeakChannel final : public IonChannel {
 public:
  explicit LeakChannel(double g = 0.3, double E = -54.387)
      : g_(g), E_(E) {}

  const char* name() const override { return "Leak"; }

  double conductance() const override { return g_; }

  double current(double V) const override {
    return g_ * (V - E_);
  }

  void step(double /*dt*/, double /*V*/) override {}

  void set_steady_state(double /*V*/) override {}

 private:
  double g_;
  double E_;
};
