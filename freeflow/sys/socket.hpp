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

#ifndef FREEFLOW_SOCKET_HPP
#define FREEFLOW_SOCKET_HPP

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <type_traits>

#include <freeflow/sys/error.hpp>
#include <freeflow/proto/ipv4.hpp>
#include <freeflow/proto/ipv6.hpp>

namespace freeflow {

// -------------------------------------------------------------------------- //
// Internet protocol support
//
// TODO: Consider moving this into a separate file.

/// A class used to encapsulate static information about address
/// families. This provides names for derived classes, to help improve
/// writability.
struct Address_info {
  enum Family : sa_family_t {
    IPv4 = AF_INET, 
    IPv6 = AF_INET6,
#if defined(BSD)
    RAW  = PF_NDRV,             // This is BSD only 
#elif defined(LINUX)
    RAW  = AF_PACKET,           // This is Linux only
#endif
  };
};

/// The type of an Internet port.
using Ip_port = in_port_t;

// =====
// Ipv4
// =====

/// An Ipv4 host adress.
struct Ipv4_addr : in_addr, Address_info { 
  static constexpr Family family = IPv4;
};


/// An Ipv4 socket address is a host and port. This class provides a
/// read-only view of the underlying system structure.
struct Ipv4_sockaddr : sockaddr_in, Address_info {
  static constexpr Family family = IPv4;

  Ipv4_sockaddr() = default;
  Ipv4_sockaddr(const Ipv4_addr&, Ip_port);
  Ipv4_sockaddr(const std::string&, Ip_port);

  Ip_port port() const;
  
  Ipv4_addr&       addr();
  const Ipv4_addr& addr() const;
};

bool operator==(const Ipv4_addr& a, const Ipv4_addr& b);
bool operator!=(const Ipv4_addr& a, const Ipv4_addr& b);

bool operator==(const Ipv4_sockaddr& a, const Ipv4_sockaddr& b);
bool operator!=(const Ipv4_sockaddr& a, const Ipv4_sockaddr& b);

// =====
// Ipv6
// =====

/// In Ipv6 host address.
struct Ipv6_addr : in6_addr, Address_info {
  static constexpr Family family = IPv6;
};

/// An Ipv6 socket address is a host and port. This class provides a
/// read-only view of the underlying system structure.
struct Ipv6_sockaddr : sockaddr_in6, Address_info {
  static constexpr Family family = IPv6;

  Ipv6_sockaddr() = default;
  Ipv6_sockaddr(const Ipv6_addr&, Ip_port);
  Ipv6_sockaddr(const std::string&, Ip_port);

  Ip_port port() const;

  Ipv6_addr&       addr();
  const Ipv6_addr& addr() const;
};

bool operator==(const Ipv6_addr& a, const Ipv6_addr& b);
bool operator!=(const Ipv6_addr& a, const Ipv6_addr& b);

bool operator==(const Ipv6_sockaddr& a, const Ipv6_sockaddr& b);
bool operator!=(const Ipv6_sockaddr& a, const Ipv6_sockaddr& b);

// -------------------------------------------------------------------------- //
// Socket address


/// The address class represents a socket address for a local or remote
/// host. The details of the address are determined by its address family.
/// Examples include IPv4 and IPv6 socket addresses, which are comprised
/// of a 32 or 128 bit host address and a 16 bit port number.
///
/// This is the primary interface for constructing and working with
/// socket addresses.
struct Address : Address_info {
  Address() = default;

  Address(Family t, const std::string& n, Ip_port p);
  Address(const Ipv4_addr& a, Ip_port p = 0);
  Address(const Ipv6_addr& a, Ip_port p = 0);

  Family family() const;

  // Returns the underlying IPv4 address.
  Ipv4_sockaddr&       as_ipv4();
  const Ipv4_sockaddr& as_ipv4() const;
  
  // Returns the underlying IPv6 address.
  Ipv6_sockaddr&       as_ipv6();
  const Ipv6_sockaddr& as_ipv6() const;

  // Returns the underlying general address.
  sockaddr*       addr();
  const sockaddr* addr() const;

  // Returns the size of the address object.
  socklen_t len() const;

  sockaddr_storage storage;
};

bool operator==(const Address& l, const Address& r);
bool operator!=(const Address& l, const Address& r);

// Printing
std::string to_string(const Address& a);


// -------------------------------------------------------------------------- //
// Socket

// The socket base class encapsulates informaiton about a socket. This
// is used to provide efficient move semantics for a Socket (since the
// state is a trivial type).
struct Socket_base : Address_info {
  enum Transport { 
    // Internet protocols
    UDP, 
    TCP,
    TLS,
    SCTP,
    
    // Raw sockets
    RAW_IPv4,
    RAW_IPv6,
    RAW_UDP,
    RAW_TCP,
    RAW_ICMPv4,
    RAW_ICMPv6,
  };

  Socket_base() = default;
  Socket_base(Family f, Transport t);
  Socket_base(int, Transport, const Address&, const Address&);

  static int type(Transport);
  static int protocol(Transport);

  int type() const;
  int protocol() const;

  Family    family;
  Transport transport;
  Address   local;
  Address   peer;
  int       fd;
  int       backlog;
};

/// The socket is an endpoint for communicating systems.
///
/// Note that sockets are resources; they can be moved but not copied. 
///
/// TODO: Document me!
struct Socket : Socket_base {
  // Not default constructible.
  Socket() = delete;

  // Not copyable.
  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  // Move semantics
  Socket(Socket&&);
  Socket& operator=(Socket&&);

  // Socket construction
  Socket(Family, Transport);
  Socket(int, Transport, const Address&, const Address&);

  ~Socket();
};

// Socket operations
Error bind(Socket& s, Address a = Address());
Error connect(Socket& s, const Address& a);
Error listen(Socket& s, int backlog = SOMAXCONN);
Socket accept(Socket& s);

int read(Socket& s);
int write(Socket& s);
int send_to(Socket& s);
int recv_from(Socket& s);

// Pretty printing
std::string to_string(const Socket& s);

} // namespace freeflow

#include "socket.ipp"

#endif
