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

#ifndef FREEFLOW_OFP_PROTOCOL_HPP
#define FREEFLOW_OFP_PROTOCOL_HPP

#include <queue>

#include <freeflow/sys/buffer.hpp>
#include <freeflow/proto/ofp/ofp.hpp>

namespace freeflow {
namespace ofp {

// A message queue represents a sequence of buffered messages
// with each buffer containing a single message. 
//
// There are parts of the interface: get and put buffer are used by the
// lower level to move data to the protocol state machine. The get
// and put message operations are used by the commications protocol
// to move data in the reverse direction.
//
// TODO: The message queue could be made protocol dependent, with the
// message interface being the only facility in this namespace.
//
// FIXME: Error handling.
struct Message_queue : std::queue<Buffer> {

  // Northbound traffic
  void put_buffer(Buffer&&);
  void get_buffer(Buffer&);

  // Southbound traffic
  template<typename H, typename P>
    void put_message(const H&, const P&);

  template<typename H>
    void peek_header(H&);

  template<typename H, typename P>
    void get_payload(const H&, P&);
};


struct Config {
  Uint8 version = 0;
};


// The Protocol_handler class represents the base class of
// state machines for specific versions of the protocol.
//
// Technically, this is a decoarator.
struct Protocol_handler {
  virtual void to_feature() = 0;
  virtual void wait_feature() = 0;
};

/// The Protocol represents phases of a more general protocol.
/// OpenFlow defines a number of distinct protocols, each exchanging
/// different messages. Each version of the standard defines a different
/// protocol. Version negotation is common to all of them.
///
/// The primary mechanism for communicating with the communications
/// controller is a pair of message queues for reading and writing.
/// Received messages are written 
///
/// TODO: These all take a time argument.
class Protocol {
  using Handler = Protocol_handler;

  enum State {
    WAIT_HELLO,
    WAIT_FEATURE,
  };

public:
  void open();
  void close();
  void message();
  void time();

  // Read/write helper functions
  template<typename H, typename P>
    void put_message(const H& h, const P& p);

  template<typename H>
    void peek_header(H& h);

  template<typename H, typename P>
    void get_payload(const H&, P& p);

  Message_queue read;
  Message_queue write;

  void to_hello();
  void wait_hello();
  void to_feature();
  void wait_feature();
  void to_established();

private:
  Handler* negotiate(Uint8);

  Config   config_;
  Uint8    version_;
  State    state_;
  Handler* proto_;
};

} // ofp
} // freeflow

#include "protocol.ipp"

#endif