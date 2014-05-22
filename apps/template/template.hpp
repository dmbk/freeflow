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

#ifndef NOCONTROL_TEMPLATE_HPP
#define NOCONTROL_TEMPLATE_HPP

#include <freeflow/sdn/application.hpp>
#include <freeflow/sdn/controller.hpp>
#include <freeflow/sdn/switch.hpp>

namespace ff = freeflow;

class Factory;
class Template;

/// The Template application
class Template : public ff::Application {
public:
  void load(ff::Controller&);
  void unload(ff::Controller&);
};


/// Create instances of the Template application.
class Factory : public ff::Application_factory {
  ff::Application* construct();
  void destroy(ff::Application*);
};

#endif