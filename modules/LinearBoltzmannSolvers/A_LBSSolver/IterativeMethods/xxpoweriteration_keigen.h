#ifndef CHITECH_XXPOWERITERATION_KEIGEN_H
#define CHITECH_XXPOWERITERATION_KEIGEN_H

#include "ChiPhysics/SolverBase/chi_solver.h"
#include "LinearBoltzmannSolvers/A_LBSSolver/lbs_solver.h"
#include "A_LBSSolver/IterativeMethods/wgs_context.h"

namespace lbs
{

class XXPowerIterationKEigen : public chi_physics::Solver
{
protected:
  LBSSolver& lbs_solver_;
  size_t max_iters_;
  double k_tolerance_;
  bool reset_solution_;

  VecDbl& q_moments_local_;
  VecDbl& phi_old_local_;
  VecDbl& phi_new_local_;
  std::vector<LBSGroupset>& groupsets_;
  AGSLinearSolver<Mat, Vec, KSP>& primary_ags_solver_;
  lbs::SetSourceFunction active_set_source_function_;
  LBSGroupset& front_gs_;
  std::shared_ptr<chi_math::LinearSolver<Mat, Vec, KSP>> front_wgs_solver_;
  std::shared_ptr<lbs::WGSContext<Mat, Vec, KSP>> front_wgs_context_;

  double k_eff_ = 1.0;
public:
  static chi_objects::InputParameters GetInputParameters();

  explicit XXPowerIterationKEigen(const chi_objects::InputParameters& params);

  void Initialize() override;
  void Execute() override;

protected:
  void SetLBSFissionSource(const VecDbl& input, bool additive);
  void SetLBSScatterSource(const VecDbl& input, bool additive,
                           bool suppress_wgs = false);
};

}

#endif // CHITECH_XXPOWERITERATION_KEIGEN_H
