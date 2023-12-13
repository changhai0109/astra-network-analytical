/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#include "common/Type.hh"
#include "common/EventList.hh"

namespace NetworkAnalytical {

/**
 * EventQueue manages scheduled EventLists.
 */
class EventQueue {
 public:
  /**
   * Constructor.
   */
  EventQueue();

  /**
   * Get current event time of the event queue.
   *
   * @return current event time
   */
  [[nodiscard]] EventTime get_current_time() const;

  /**
   * Check all registered events are invoked.
   * i.e., check if the event queue is empty.
   *
   * @return true if the event queue is empty, false otherwise
   */
  [[nodiscard]] bool finished() const;

  /**
   * Proceed the event queue.
   * i.e., first update the current event time to the next registered event time,
   * and then invoke all events registered at the current updated event time.
   */
  void proceed();

  /**
   * Schedule an event with a given event time.
   *
   * @param event_time time of event
   * @param callback callback function pointer
   * @param callback_arg argument of the callback function
   */
  void schedule_event(
      EventTime event_time,
      Callback callback,
      CallbackArg callback_arg);

 private:
  /// current time of the event queue
  EventTime current_time;

  /// list of EventLists
  std::list<EventList> event_queue;
};

} // namespace NetworkAnalytical
