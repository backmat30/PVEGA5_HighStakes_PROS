#include "pvegas/pros_adapters/ProsTask.hpp"

#include <memory>

#include "pros/rtos.hpp"

namespace pvegas {
namespace pros_adapters {
void ProsTask::start(void (*function)(void *), void *params) {
  // defines the task
  task = std::make_unique<pros::Task>(function, params);
}

void ProsTask::remove() {
  // removes the task from the system if it exists
  if (task) {
    task->remove();
  }
}

void ProsTask::suspend() {
  // attempts to suspend the task if it exists
  if (task) {
    task->suspend();
  }
}

void ProsTask::resume() {
  // attempts to resume the task if it exists
  if (task) {
    task->resume();
  }
}

void ProsTask::join() {
  // calls the join function if the task exists
  if (task) {
    task->join();
  }
}
}  // namespace pros_adapters
}  // namespace pvegas