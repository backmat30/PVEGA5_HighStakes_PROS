#ifndef __I_CLAMP_HPP__
#define __I_CLAMP_HPP__

namespace driftless {
namespace robot {
namespace subsystems {
namespace clamp {
class IClamp {
 public:
  // destroy the clamp object
  virtual ~IClamp() = default;

  // initialize the clamp
  virtual void init() = 0;

  // run the clamp
  virtual void run() = 0;

  // set the state of the clamp
  virtual void setState(bool clamped) = 0;

  // get the state of the clamp
  virtual bool getState() = 0;
};
}  // namespace clamp
}  // namespace subsystems
}  // namespace robot
}  // namespace driftless
#endif