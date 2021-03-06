# Copyright (c) 2013-2014 Flowgrammable.org
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an "AS IS"
# BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
# or implied. See the License for the specific language governing
# permissions and limitations under the License.

set(src fd_utils.cpp
        meta.cpp
        data.cpp
        error.cpp
        time.cpp
        signal.cpp
        path.cpp
        resource.cpp
        file.cpp
        socket.cpp
        handler.cpp
        timer.cpp
        selector.cpp
        reactor.cpp
        acceptor.cpp
        connector.cpp
        buffer.cpp
        syslog.cpp
        pipe.cpp
        json.cpp
        library.cpp
        print.cpp
        cli.cpp)

if(BSD)
  LIST(APPEND src kqueue.cpp)
elseif(LINUX)
  LIST(APPEND src epoll.cpp)
endif()

# Header and inline files are installed.
set(hdr fd_utils.hpp
        meta.hpp
        data.hpp
        error.hpp     error.ipp
        time.hpp      time.ipp
        signal.hpp    signal.ipp
        path.hpp
        resource.hpp  resource.ipp
        file.hpp      file.ipp
        socket.hpp    socket.ipp
        handler.hpp   handler.ipp
        timer.hpp     timer.ipp
        selector.hpp  selector.ipp
        reactor.hpp   reactor.ipp
        acceptor.hpp  acceptor.ipp
        connector.hpp connector.ipp
        buffer.hpp    buffer.ipp
        syslog.hpp
        pipe.hpp
        json.hpp      json.ipp
        library.hpp   library.ipp
        print.hpp     print.ipp
        cli.hpp       cli.ipp)

if(BSD)
  LIST(APPEND hdr ${kqueue.hpp})
elseif(LINUX)
  LIST(APPEND hdr ${epoll.hpp})
endif()


# --------------------------------------------------------------------------- //
# Targets

add_shared_library(freeflow ${src})
target_link_libraries(freeflow ${CMAKE_DL_LIBS})


# --------------------------------------------------------------------------- //
# Installation

install(FILES ${hdr} DESTINATION include/freeflow/sys)
install(TARGETS freeflow LIBRARY DESTINATION lib)


# --------------------------------------------------------------------------- //
# Examples
add_subdirectory(examples)


# --------------------------------------------------------------------------- //
# Testing

add_subdirectory(data.test)
add_subdirectory(signal.test)
add_subdirectory(socket.test)
add_subdirectory(reactor.test)
add_subdirectory(acceptor.test)
add_subdirectory(connector.test)
add_subdirectory(library.test)
add_subdirectory(json.test)

