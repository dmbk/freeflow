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

#ifndef FREEFLOW_EPOLL_HPP
#define FREEFLOW_EPOLL_HPP

#include <unistd.h>
#include <sys/epoll.h>

#include <map>
#include <vector>

#include "freeflow/sys/error.hpp"
#include "freeflow/sys/time.hpp"

namespace freeflow {

struct EPevent : epoll_event
{
  enum Type : uint32_t {
    READ  = EPOLLIN,
    WRITE = EPOLLOUT,
    CLOSED = EPOLLRDHUP,
    EDGE = EPOLLET,
    ONESHOT = EPOLLONESHOT,
  };

  EPevent() = default;
  EPevent(const EPevent&) = default;
  EPevent(Type t, int fd);
};

struct Epoll
{
  Epoll();
  ~Epoll();

  int fd;

  std::map<int,EPevent> watched;
};

Error add_reader(Epoll& e, int fd);
Error add_writer(Epoll& e, int fd);
Error del_reader(Epoll& e, int fd);
Error del_writer(Epoll& e, int fd);

using EPevents = std::vector<EPevent>;
EPevents poll(Epoll& ep, const MicroTime& mt);

} // namespace freeflow

#include "epoll.ipp"

#endif
