#include "TimeStepper.h"

#include <cmath>

namespace chi_physics
{

chi::InputParameters TimeStepper::GetInputParameters()
{
  chi::InputParameters params = ChiObject::GetInputParameters();

  params.AddOptionalParameter("dt", 0.01, "Initial timestep to use");
  params.AddOptionalParameter("time", 0.0, "Initial time");
  params.AddOptionalParameter(
    "time_index", 0, "Time index. Useful for output control.");
  params.AddOptionalParameter("start_time", 0.0, "Start time");
  params.AddOptionalParameter("end_time", 1.0, "End time");
  params.AddOptionalParameter(
    "max_time_steps",
    -1,
    "Maximum number of timesteps to take. A negative number disables this.");
  params.AddOptionalParameter("dt_min", 1.0e-5, "Minimum allowable timestep.");

  params.AddOptionalParameter("eps",
                              1.0e-8,
                              "General time tolerance. This is used in "
                              "fuzzy equals.");

  return params;
}

TimeStepper::TimeStepper(const chi::InputParameters& params)
  : ChiObject(params),
    dt_(params.GetParamValue<double>("dt")),
    time_(params.GetParamValue<double>("time")),
    t_index_(params.GetParamValue<size_t>("time_index")),

    start_time_(params.GetParamValue<double>("start_time")),
    end_time_(params.GetParamValue<double>("end_time")),
    max_time_steps_(params.GetParamValue<int>("max_time_steps")),
    dt_min_(params.GetParamValue<double>("dt_min")),

    general_tolerance_(params.GetParamValue<double>("eps")),
    last_dt_(dt_)
{
}

/**Overridable method to get the timestep size.*/
double TimeStepper::TimeStepSize() const { return dt_; }

/**Returns the current controller time.*/
double TimeStepper::Time() const { return time_; }

/**Returns the current time index.*/
size_t TimeStepper::TimeStepIndex() const { return t_index_; }

/**Returns the current controller start_time.*/
double TimeStepper::StartTime() const { return start_time_; }

/**Returns the current controller end_time.*/
double TimeStepper::EndTime() const { return end_time_; }

/**Returns the current controller max time steps.*/
double TimeStepper::MaxTimeSteps() const { return max_time_steps_; }

/**If start_time <= time <= end_time, this will return true.*/
bool TimeStepper::IsActive() const
{
  if (max_time_steps_ >= 0 and (t_index_ >= max_time_steps_)) return false;

  bool active = (time_ >= (start_time_ - general_tolerance_) and
                 time_ <= (end_time_ + general_tolerance_));

  if (std::fabs(end_time_ - time_) < general_tolerance_) active = false;

  return active;
}

/**Manually set the time step size.*/
void TimeStepper::SetTimeStepSize(double dt)
{
  dt_ = dt;

  dt_ = std::min(end_time_ - time_, dt_);
}

/**Manually set the current time.*/
void TimeStepper::SetTime(double time) { time_ = time; }

/**Manually set the start_time.*/
void TimeStepper::SetStartTime(double time) { start_time_ = time; }

/**Manually set the end_time.*/
void TimeStepper::SetEndTime(double time)
{
  end_time_ = time;
  dt_ = last_dt_;
}

/**Manually set the maximum number of time steps. A negative number disables
 * this check.*/
void TimeStepper::SetMaxTimeSteps(int n) { max_time_steps_ = n; }

/**Manually sets the minimum time step size.*/
void TimeStepper::SetMinimumTimeStepSize(double dt_min) { dt_min_ = dt_min; }

/**Advances the controller's state. The most basic action here is to
 * advance time and the time index. If the solver is at or beyond its end time
 * then it will return false. Otherwise it will advance and return true.*/
void TimeStepper::Advance()
{
  last_dt_ = dt_;
  time_ += dt_;
  t_index_ += 1;

  // Still trying to figure this part out
  // Limit dt to not exceed end_time_
  // dt_ = std::min(end_time_ - time_, dt_);
  dt_ = std::max(dt_, dt_min_);
}

std::string TimeStepper::StringTimeInfo(bool old_time /*=false*/) const
{
  const double time = old_time ? time_ : time_ + dt_;
  std::stringstream outstr;
  outstr << "Time step " << t_index_;
  {
    char buffer[100];
    snprintf(buffer, 100, ", time = %g", time);
    outstr << buffer;
  }
  {
    char buffer[100];
    snprintf(buffer, 100, ", dt = %g", dt_);
    outstr << buffer;
  }

  return outstr.str();
}

} // namespace chi_physics