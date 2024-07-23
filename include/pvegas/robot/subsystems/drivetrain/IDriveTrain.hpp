#ifndef __I_DRIVETRAIN_HPP__
#define __I_DRIVETRAIN_HPP__

#include "Velocity.hpp"
namespace pvegas {
namespace robot {
namespace subsystems {
namespace drivetrain {
    class IDriveTrain{
        public:
        virtual ~IDriveTrain() = default;

        virtual void init() = 0;

        virtual void run() = 0;

        virtual Velocity getVelocity() = 0;

        virtual void setVelocity(Velocity velocity) = 0;

        virtual void setVoltage(double leftVoltage, double rightVoltage) = 0;
        
        virtual double getRadius() = 0;
    };
}
} // namespace subsystems
} // namespace robot
} // namespace pvegas
#endif