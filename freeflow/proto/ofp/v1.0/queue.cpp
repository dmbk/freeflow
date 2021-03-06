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

#include "queue.hpp"

namespace freeflow {
namespace ofp {
namespace v1_0 {

// To view
Error
to_view(View& v, const Rate_property& m) {
  to_view(v, m.rate);
  pad(v, 6);
  return {};
}

Error
to_view(View& v, const Property_value& m, Property_type t) {
  return to_view(v, m.rate);
}

Error
to_view(View& v, const Property& m) {
  if (not is_valid(m.property)) // Requried semantic check
    return make_error_code(errc::bad_property_type);
  if (m.length < bytes(m)) // Required semantic check
    return make_error_code(errc::bad_property_length);
  
  to_view(v, m.property);
  to_view(v, m.length);
  pad(v, 4);

  if (Constrained_view c = constrain(v, m.length - bytes(m)))
    return to_view(c, m.value, m.property);
  else
    return {};
}

Error
to_view(View& v, const Queue& m) {
  if (remaining(v) < bytes(m))
    return make_error_code(errc::property_overflow);
  if (m.length < bytes(m))
    return make_error_code(errc::bad_queue_length);
  to_view(v, m.queue_id);
  to_view(v, m.length);
  pad(v, 2);
  to_view(v, m.properties);
  return {};
}

// From view
Error
from_view(View& v, Rate_property& m) {
  from_view(v, m.rate);
  pad(v, 6);
  return {};
}

Error
from_view(View& v, Property_value& m, Property_type t) {
  return from_view(v, m.rate);
}

Error
from_view(View& v, Property& m) {
  from_view(v, m.property);
  from_view(v, m.length);
  pad(v, 4);

  if (not is_valid(m.property)) // Required semantic check
    return make_error_code(errc::bad_property_type);
  if (m.length < bytes(m)) // Required semantic check
    return make_error_code(errc::bad_property_length);

  if (Constrained_view c = constrain(v, m.length - bytes(m)))
    return to_view(c, m.value, m.property);
  else
    return {};
}

Error
from_view(View& v, Queue& m) {
  if (remaining(v) < bytes(m))
    return make_error_code(errc::property_overflow);
  if (m.length < bytes(m))
    return make_error_code(errc::bad_queue_length);
  from_view(v, m.queue_id);
  from_view(v, m.length);
  pad(v, 2);
  from_view(v, m.properties);
  return {};
}
} // namespace v1_0
} // namespace ofp
} // namespace freeflow
