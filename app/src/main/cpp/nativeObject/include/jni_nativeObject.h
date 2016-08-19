/**
 * Copyright (c) 2015, Alov Maxim <alovmax@yandex.ru>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 * and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided with
 * the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SURFACE3D_JNI_NATIVEOBJECT_H_
#define SURFACE3D_JNI_NATIVEOBJECT_H_

#include <map>
#include <memory>
#include <string>

#include "logger.h"
#include "NativeObject.h"
#include "Scene.h"
#include "Signatures.h"


namespace native {

template <typename T>
NativeObject* createType() {
  return new T();
}

class NativeObjectFactory {
public:
  typedef NativeObject* (*ComponentFactoryFuncPtr)();
  typedef std::map<std::string, ComponentFactoryFuncPtr> map_type;

  NativeObjectFactory();
  virtual ~NativeObjectFactory();

  template <typename T>
  void AddType(const std::string& componentName) {
    ComponentFactoryFuncPtr function = &createType<T>;
    m_Map.insert(std::make_pair(componentName, function));
  }

  NativeObject* createObject(const std::string& componentName) {
    auto iterator = m_Map.find(componentName);
    if (iterator != m_Map.end()) {
      return iterator->second ();  // instantiation
    } else {
      WRN("No object has been created!");
      return nullptr;
    }
  }

private:
  map_type m_Map;
};

extern std::unique_ptr<NativeObjectFactory> Factory;

}  // namespace native

#endif /* SURFACE3D_JNI_NATIVEOBJECT_H_ */
