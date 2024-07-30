#include "pvegas/pros_adapters/ProsDelayer.hpp"

namespace pvegas{
    namespace pros_adapters{
        std::unique_ptr<ProsDelayer> ProsDelayer::clone() const{
            return std::unique_ptr<ProsDelayer>(std::make_unique<ProsDelayer>(*this));
        }

        void ProsDelayer::delay(uint32_t millis){
            pros::Task::delay(millis);
        }

        void ProsDelayer::delayUntil(uint32_t time){
            uint32_t currentTime{pros::millis()};
            pros::Task::delay_until(&currentTime, time - currentTime);
        }
    }
}