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

inline
Transfer::Transfer(int f, uint8_t* b, std::size_t n) :
  fd(f), buff(b), n_bytes(n), fd_state(1), n_transferred(0)
{ }

inline
Transfer::operator int()
{
  return fd_state < 0 ? fd_state : n_transferred;
}

inline int
transfer_available(Transfer& t, ssize_t (*op)(int, void*, size_t))
{
  // Read the remainder to the buff
  t.fd_state = op(t.fd, t.buff+t.n_transferred, t.n_bytes-t.n_transferred);
  // If successful update bytes read
  if (t.fd_state > 0)
    t.n_transferred += t.fd_state;
  // Return fd state on failure or number of bytes read
  return t.fd_state < 0 ? t.fd_state : t.n_transferred;
}

inline int
transfer_all(Transfer& t, ssize_t (*op)(int, void*, size_t))
{
  // More to read and fd state good
  while(t.n_transferred < t.n_bytes and t.fd_state > 0) {
    // Read the remainder to the buff
    t.fd_state = op(t.fd, t.buff+t.n_transferred, t.n_bytes-t.n_transferred);
    // If successful update bytes read
    if (t.fd_state > 0)
      t.n_transferred += t.fd_state;
  }
  // Return fd state on failure or number of bytes read
  return t.fd_state < 0 ? t.fd_state : t.n_transferred;
}

inline int
read_available(Transfer& t)
{
  return transfer_available(t, ::read);
}

inline int
read_all(Transfer& t)
{
  return transfer_all(t, ::read);
}

inline int
write_available(Transfer& t)
{
  return transfer_available(t, (ssize_t (*)(int, void*, size_t))::write);
}

inline int
write_all(Transfer& t)
{
  return transfer_all(t, (ssize_t (*)(int, void*, size_t))::write);
}

} // namespace freeflow
