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

#include "timer_queue.hpp"

namespace nocontrol {

using namespace ff;

// Dispatch any events whose timers have expired.
void
Timer_queue::dispatch() {
  Time_point t = now();
  while (not empty() and top().time < t) {
    top().handler->on_time();
    pop();
  }
}

} // namespace nocontrol