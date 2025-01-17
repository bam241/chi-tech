#ifndef CHI_FFINTER_VOLUME_H
#define CHI_FFINTER_VOLUME_H

#include "../chi_ffinterpolation.h"
#include "mesh/LogicalVolume/LogicalVolume.h"

#include <petscksp.h>

//###################################################################
/**Volume-wise field function interpolation.
 *
 * This interpolator allows the user to obtain quantities by logical
 * volume. If no logical volume is assigned to the method it will
 * default to operating over the entire volume.\n
 * \n
 * The method also supports a few primitive operations:
 *  - OP_VOLUME_AVG. Obtains the volume average of the field function
 *    of interest.
 *  - OP_VOLUME_SUM. Obtains the volume integral of the field function
 *    of interest.*/
class chi_mesh::FieldFunctionInterpolationVolume :
  public chi_mesh::FieldFunctionInterpolation
{
protected:
  std::shared_ptr<chi_mesh::LogicalVolume> logical_volume_ = nullptr;
  ff_interpolation::Operation op_type_ = ff_interpolation::Operation::OP_SUM;
  std::string op_lua_func_;
  double op_value_ = 0.0;

private:
  std::vector<uint64_t> cell_local_ids_inside_logvol_;

public:
  FieldFunctionInterpolationVolume() :
    FieldFunctionInterpolation(ff_interpolation::Type::VOLUME)
  {  }
  std::shared_ptr<chi_mesh::LogicalVolume>&
  GetLogicalVolume() {return logical_volume_;}

  ff_interpolation::Operation& GetOperationType() {return op_type_;}

  std::string& GetOperationLuaFunction() {return op_lua_func_;}

  double& GetOpValue() {return op_value_;}

  //01
  void Initialize() override;

  //02
  void Execute() override;

  double CallLuaFunction(double ff_value, int mat_id) const;

  std::string GetDefaultFileBaseName() const override
  {return "ZVFFI";}
  void ExportPython(std::string base_name) override {}
};


#endif
