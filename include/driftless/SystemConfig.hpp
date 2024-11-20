#ifndef __SYSTEM_CONFIG_HPP__
#define __SYSTEM_CONFIG_HPP__

#include <memory>

#include "driftless/alliance/IAlliance.hpp"
#include "driftless/auton/IAuton.hpp"
#include "driftless/config/IConfig.hpp"
#include "driftless/profiles/IProfile.hpp"

namespace driftless {
struct SystemConfig {
  std::shared_ptr<alliance::IAlliance> alliance{};

  std::unique_ptr<auton::IAuton> auton{};

  std::unique_ptr<config::IConfig> config{};

  std::unique_ptr<profiles::IProfile> profile{};
};
}  // namespace driftless
#endif