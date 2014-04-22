// Copyright (c) 2013-2014 Flowgrammable, LLC.
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

#ifndef FREEFLOW_CONTROLLER_HPP
#define FREEFLOW_CONTROLLER_HPP

#include <unordered_set>

#include <freeflow/sys/socket.hpp>
#include <freeflow/nbi/application.hpp>

namespace freeflow {

struct Switch;

/// The Controller class represents...
///
/// \todo The application currently hosts a single application. Obviously
/// it should host more.
class Controller {
  using Switch_set = std::unordered_set<Switch*>;

public:

  // Application management.
  template<typename T> bool load();
  template<typename T> bool unload();

  // Switch management
  Switch& connect(Socket&);
  void disconnect(Switch&);

private:
  Application* app_;      // The hosted applicatoin.
  Switch_set   switches_;

};

} // namespace freeflow

#include "controller.ipp"

#endif

