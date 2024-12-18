#ifndef __I_CLOCK_HPP__
#define __I_CLOCK_HPP__

#include <cstdint>
#include <memory>
namespace driftless {
namespace rtos {
class IClock {
 public:
  virtual ~IClock() = default;

  virtual std::unique_ptr<IClock> clone() const = 0;

  virtual uint32_t getTime() = 0;
};
}  // namespace rtos
}  // namespace driftless
#endif