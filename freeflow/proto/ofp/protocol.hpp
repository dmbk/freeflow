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
#include <freeflow/sys/time.hpp>
#include <freeflow/sys/reactor.hpp>
#include <freeflow/proto/ofp/ofp.hpp>

namespace freeflow {
namespace ofp {

/// A message queue represents a sequence of buffered messages
/// with each buffer containing a single message. 
///
/// There are parts of the interface: get and put buffer are used by the
/// lower level to move data to the protocol state machine. The get
/// and put message operations are used by the commications protocol
/// to move data in the reverse direction.
///
/// \todo The message queue could be made protocol dependent, with the
/// message interface being the only facility in this namespace.
///
/// \todo Implement error handling.
struct Message_queue : std::queue<Buffer> {

  void put_buffer(Buffer&&);
  void get_buffer(Buffer&);

  template<typename H, typename P>
    Error put_message(const H&, const P&);

  template<typename H>
    Error peek_header(H&);

  template<typename H, typename P>
    Error get_payload(const H&, P&);
};


/// The Config class represents that runtime configuration of
/// OpenFlow support.
///
/// \todo This needs to be read from a configuration file and
/// populated dynamically.
struct Config {
  /// The highest version of the protocol supported.
  Uint8 version = 0;

  /// Connection timeout for echo messages.
  Seconds echo_timeout = 60_s;

  /// The default idle timeout for flows.
  Seconds idle_timeout = 5_s;

  /// The default hard timeout for flows.
  Seconds hard_timeout = 10_s;
};


// The Protocol_handler class represents the base class of
// state machines for specific versions of the protocol.
//
// Technically, this is a decoarator.
//
// \todo I'm not sure I actually like this design.
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
  using Version = Protocol_handler;

  enum State {
    CLOSED,
    HELLO,       // Version negotiation
    FEATURE,     // Feature discovery
    ESTABLISHED, // Basic processing
  };

public:
  Protocol(Handler*);

  bool on_open(Reactor&);
  bool on_close(Reactor&);
  bool on_recv(Reactor&);
  bool on_time(Reactor&, int);

  // Read/write helper functions
  template<typename P>
    Error put_message(const P& p);

  template<typename H>
    Error peek_header(H& h);

  template<typename H, typename P>
    Error get_payload(const H&, P& p);

  // Message queue
  Message_queue read;
  Message_queue write;

private:
  Uint32 xid();
  Version* negotiate(Uint8);
  bool ping(Reactor&);

  // Open state and transitions
  bool open_to_hello(Reactor&);
  
  // Initial state and transitions
  bool hello_recv(Reactor&);
  bool hello_time(Reactor&);
  bool hello_to_feature(Reactor&);
  bool hello_to_close(Reactor&);
  
  // Discover state and transitions
  bool feature_recv(Reactor&);
  bool feature_time(Reactor&);
  bool feature_to_established(Reactor&);
  bool feature_to_close(Reactor&);

  // Established state and transitions
  bool established_recv(Reactor&);
  bool established_recv_echo(Reactor&);
  bool established_time(Reactor&, int);
  bool established_to_close(Reactor&);

  Handler* handler_;
  Config   config_;
  Uint8    version_;
  State    state_;
  Version* proto_;

  Uint32   xid_;       // The curent transaction id
  int      timer_ = 0; // The timeout timer
  int      echo_ = 1;  // The ping timer
};

} // ofp
} // freeflow

#include "protocol.ipp"

#endif
