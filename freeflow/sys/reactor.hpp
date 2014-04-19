// Copyright (c) 2013-2014 Flowgrammable.org
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
// http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an "AS IS"
// BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing
// permissions and limitations under the License.

#ifndef FREEFLOW_REACTOR_HPP
#define FREEFLOW_REACTOR_HPP

#include <freeflow/sys/handler.hpp>
#include <freeflow/sys/timer.hpp>

namespace freeflow {

/// The Reactor class implements an event processor that notifies handlers
/// of resource events and availability, timer expiration, and signals.
class Reactor {
public:
  Reactor();
  ~Reactor();

  // Handlers
  void add_handler(Handler*);
  void remove_handler(Handler*);

  // Timers
  void schedule_timer(Handler*, int, Microseconds);
  void reschedule_timer(Handler*, int, Microseconds);
  void cancel_timer(Handler*, int);

  // Control
  void run();
  void stop();

private:
  bool             running_;
  Handler_registry handlers_;
  Timer_queue      timers_;
};

} // namesapce freeflow

#include <freeflow/sys/reactor.ipp>

#endif
