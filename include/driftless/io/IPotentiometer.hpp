#ifndef __I_POTENTIOMETER_HPP__
#define __I_POTENTIOMETER_HPP__

namespace driftless {
namespace io {
class IPotentiometer {
 public:
  // destroys the potentiometer object
  virtual ~IPotentiometer() = default;

  // initialize the potentiometer object
  virtual void init() = 0;

  // calibrate the sensor
  virtual void calibrate() = 0;

  // get the angle of the potentiometer
  virtual double getAngle() = 0;
};
}  // namespace io
}  // namespace driftless
#endif