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

namespace freeflow {
namespace ofp {
namespace v1_0 {

/// The v1.0 OFP state machine for controllers.
inline
Machine::Machine(Channel& ch, Controller* ctrl)
  : ch_(ch), ctrl_(ctrl) { }


/// Returns a new transaction id for the message.
///
/// FIXME: This is totally broken.
inline Uint32
Machine::xid() { return 0; }

template<typename T>
  inline void 
  Machine::push_msg(const T& x) {
    Header h { T::Kind, Uint16(bytes(h) + bytes(x)), xid() };
    ch_.send.push_msg(h, x);
  }


inline Error
Machine::send_hello(const Buffer& data) {
  push_msg(Hello {data});
  return {};
}

inline Error
Machine::send_error(Error::Hello_failed c) {
  push_msg(Error {Error::HELLO_FAILED, c});
  return {};
}

} // namespace v1_0
} // namespace ofp
} // namespace nocontrol