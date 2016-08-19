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

#include <jni.h>
#include "jni_nativeObject.h"
#include "logger.h"
#include "NativeObject.h"
#include "Signatures.h"


JNIEXPORT jlong JNICALL Java_com_orcchg_surface3d_NativeObject_initNative
  (JNIEnv *jenv, jobject, jstring clazz) {
  const char* clazz_str = jenv->GetStringUTFChars(clazz, 0);
  jlong descriptor = (jlong)(intptr_t) native::Factory->createObject(clazz_str);
  jenv->ReleaseStringUTFChars(clazz, clazz_str);
  return descriptor;
}

JNIEXPORT void JNICALL Java_com_orcchg_surface3d_NativeObject_finalizeNative
  (JNIEnv *, jobject, jlong descriptor) {
  DBG("Finalize NativeObject instance");
  native::NativeObject* ptr = (native::NativeObject*) descriptor;
  delete ptr;
}

namespace native {

NativeObject::NativeObject() {
  DBG("NativeObject::ctor");
}

NativeObject::~NativeObject() {
  DBG("NativeObject::~dtor");
}

}  // namespace native
