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

#include <iostream>

#include <freeflow/sys/json.hpp>

using namespace freeflow;

void
test_equality() {
  // bool with bool
  json::Value b1 = true;
  json::Value b2 = false;
  json::Value b3 = true;
  
  assert(b1 == b3);
  assert(b1 != b2);
  
  // int with int
  json::Value z1 = 10;
  json::Value z2 = 50;
  json::Value z3 = 10;
  
  assert(z1 == z3);
  assert(z1 != z2);
  
  // real with real
  json::Value r1 = 3.1415;
  json::Value r2 = 1.234;
  json::Value r3 = 3.1415;
  
  assert(r1 == r3);
  assert(r1 != r2);
  
  // null with null
  json::Value n1;
  json::Value n2;
  
  assert(n1 == n2);
  
  // array with array
  json::Value a1 = json::Array(std::initializer_list<json::Value>{1,2.5,"three",4,true});
  json::Value a2 = json::Array(std::initializer_list<json::Value>{"hello",9,2,5,6});
  json::Value a3 = json::Array(std::initializer_list<json::Value>{1,2.5,"three",4,true});
  
  assert(a1 == a3);
  assert(a1 != a2);
  
  // string with string
  json::Value s1 = "hello world";
  json::Value s2 = "greetings planet";
  json::Value s3 = "hello world";
  
  assert(s1 == s3);
  assert(s1 != s2);
  
  // object with object
  json::Value o1 = json::Object({{"array", a1}, {"real", r1}, {"bool",b1}});
  json::Value o2 = json::Object({{"array", a1}, {"real", r1}});
  json::Value o3 = o1;
  
  json::Value values[] = {a1,b1,r1,s1,o1,z1,n1};
  const int total = sizeof(values) / sizeof(json::Value);

  // Check every object against all other objects
  // If they are the same object, they should be equal
  // If they are of different types, they should not be equal
  for (int i = 0;i < total;i++)
    for (int d = 0;d < total;d++)
      if (i == d)
        assert(values[i] == values[d]);
      else
        assert(values[i] != values[d]);
  
}

int main(int argc, char *argv[]) {
  test_equality();
  
  return 0;
}
