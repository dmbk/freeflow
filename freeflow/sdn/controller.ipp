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

namespace freeflow {

/// Load an application of the given type, returning true when the
/// application loads successfully. If the application does not load,
/// it is not installed.
template<typename T>
  inline void
  Controller::load() {
    app_ = new T();
    app_->load(*this);
  }

/// Unload the application of the given type.
template<typename T>
  inline void
  Controller::unload() {
    assert(dynamic_cast<T*>(app_));
    app_->unload(*this);
    delete app_;
    app_ = nullptr;
  }

} // namespace freeflow
