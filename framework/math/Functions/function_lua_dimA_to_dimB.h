#ifndef CHITECH_FUNCTION_LUA_DIMA_TO_DIMB_H
#define CHITECH_FUNCTION_LUA_DIMA_TO_DIMB_H

#include "function_dimA_to_dimB.h"

namespace chi_math::functions
{

class LuaDimAToDimB : public FunctionDimAToDimB
{
private:
  const std::string lua_function_name_;
public:
  static chi::InputParameters GetInputParameters();

  explicit LuaDimAToDimB(const chi::InputParameters& params);

  std::vector<double>
  Evaluate(const std::vector<double>& vals) const override;

  bool HasSlope() const override {return false;}
  bool HasCurvature() const override {return false;}
};

}

#endif // CHITECH_FUNCTION_LUA_DIMA_TO_DIMB_H
