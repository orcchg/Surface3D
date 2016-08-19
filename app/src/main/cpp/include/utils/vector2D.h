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

#ifndef SURFACE3D_VECTOR2D_H_
#define SURFACE3D_VECTOR2D_H_


#include <cmath>
#include <iostream>


namespace utils {

template <typename T>
class Vector2D;

template <class T>
bool operator !=(const Vector2D<T>& p1, const Vector2D<T>& p2) {
  return (p1[0] != p2[0] || p1[1] != p2[1]);
}

template <class T>
const Vector2D<T> operator *(const Vector2D<T>& p, float factor) {
  return Vector2D<T>(p[0] * factor, p[1] * factor);
}

template <class T>
const Vector2D<T> operator *(float factor, const Vector2D<T>& p) {
  return Vector2D<T>(p[0] * factor, p[1] * factor);
}

template <class T>
const Vector2D<T> operator *(const Vector2D<T>& p1, const Vector2D<T>& p2) {
  return Vector2D<T>(p1[0] * p2[0], p1[1] * p2[1]);
}

template <class T>
const Vector2D<T> operator +(const Vector2D<T>& p1, const Vector2D<T>& p2) {
  return Vector2D<T>(p1[0] + p2[0], p1[1] + p2[1]);
}

template <class T>
const Vector2D<T> operator -(const Vector2D<T>& p1, const Vector2D<T>& p2) {
  return Vector2D<T>(p1[0] - p2[0], p1[1] - p2[1]);
}

template <class T>
const Vector2D<T> operator -(const Vector2D<T>& p) {
  return Vector2D<T>(-p[0], -p[1]);
}

template <class T>
const Vector2D<T> operator /(const Vector2D<T>& p, float divisor) {
  return Vector2D<T>(p[0] / divisor, p[1] / divisor);
}

template <class T>
bool operator ==(const Vector2D<T>& p1, const Vector2D<T>& p2) {
  return (p1[0] == p2[0] && p1[1] == p2[1]);
}

template <class T>
bool operator <(const Vector2D<T>& a, const Vector2D<T>& b) {
  return (a[0] < b[0] && a[1] < b[1]);
}

template <class T>
bool operator >=(const Vector2D<T>& a, const Vector2D<T>& b) {
  return (a[0] >= b[0] || a[1] >= b[1]);
}

/**
 * Vector in 3 dimensions, with basics operator s overloaded.
 */
template <typename T>
class Vector2D {
 public:
  inline Vector2D(void) {
    p[0] = p[1] = T();
  }

  inline Vector2D(T p0, T p1) {
    p[0] = p0;
    p[1] = p1;
  }

  inline Vector2D(const Vector2D & v) {
    init(v[0], v[1]);
  }

  inline Vector2D(T* pp) {
    p[0] = pp[0];
    p[1] = pp[1];
  }

  // ---------
  // operator s
  // ---------
  inline T& operator [](int Index) {
    return (p[Index]);
  }

  inline const T& operator [](int Index) const {
    return (p[Index]);
  }

  inline Vector2D& operator =(const Vector2D& P) {
    p[0] = P[0];
    p[1] = P[1];
    return (*this);
  }

  inline Vector2D& operator +=(const Vector2D& P) {
    p[0] += P[0];
    p[1] += P[1];
    return (*this);
  }

  inline Vector2D& operator -=(const Vector2D& P) {
    p[0] -= P[0];
    p[1] -= P[1];
    return (*this);
  }

  inline Vector2D& operator *=(const Vector2D& P) {
    p[0] *= P[0];
    p[1] *= P[1];
    return (*this);
  }

  inline Vector2D& operator *=(T s) {
    p[0] *= s;
    p[1] *= s;
    return (*this);
  }

  inline Vector2D& operator /=(const Vector2D& P) {
    p[0] /= P[0];
    p[1] /= P[1];
    return (*this);
  }

  inline Vector2D& operator /=(T s) {
    p[0] /= s;
    p[1] /= s;
    return (*this);
  }


  //---------------------------------------------------------------
  inline Vector2D& init(T x, T y) {
    p[0] = x;
    p[1] = y;
    return (*this);
  }

  inline T getSquaredLength() const {
    return (dotProduct(*this, *this));
  }

  inline T getLength() const {
    return (T) sqrt(getSquaredLength());
  }

  /// Return length after normalization
  inline T normalize(void) {
    T length = getLength();
    if (length == 0.0f)
      return 0;
    T rezLength = 1.0f / length;
    p[0] *= rezLength;
    p[1] *= rezLength;
    return length;
  }

  inline void fromTo(const Vector2D& P1, const Vector2D& P2) {
    p[0] = P2[0] - P1[0];
    p[1] = P2[1] - P1[1];
  }

  inline float transProduct(const Vector2D& v) const {
    return (p[0] * v[0] + p[1] * v[1]);
  }

  inline Vector2D projectOn(const Vector2D& N, const Vector2D& P) const {
    T w = dotProduct(((*this) - P), N);
    return (*this) - (N * w);
  }

  static inline Vector2D segment(const Vector2D& a, const Vector2D& b) {
    Vector2D r;
    r[0] = b[0] - a[0];
    r[1] = b[1] - a[1];
    return r;
  }

  static inline T dotProduct(const Vector2D& a, const Vector2D& b) {
    return (a[0] * b[0] + a[1] * b[1]);
  }

  static inline T squaredDistance(const Vector2D& v1, const Vector2D& v2) {
    Vector2D tmp = v1 - v2;
    return (tmp.getSquaredLength());
  }

  static inline T distance(const Vector2D& v1, const Vector2D& v2) {
    Vector2D tmp = v1 - v2;
    return (tmp.getLength());
  }

  static inline Vector2D interpolate(const Vector2D& u, const Vector2D& v, T alpha) {
    return (u * (1.0f - alpha) + v * alpha);
  }

  static inline Vector2D projectOntoVector(const Vector2D& v1, const Vector2D& v2) {
    return v2 * dotProduct(v1, v2);
  }

  inline Vector2D transformIn(const Vector2D& pos, const Vector2D& n, const Vector2D& u, const Vector2D& v) const {
    Vector2D q = (*this) - pos;
    return Vector2D(u[0] * q[0] + u[1] * q[1], v[0] * q[0] + v[1] * q[1], n[0] * q[0] + n[1] * q[1]);
  }

protected:
  T p[2];
};

template <class T>
inline Vector2D<T> swap(Vector2D<T>& P, Vector2D<T>& Q) {
  Vector2D<T> tmp = P;
  P = Q;
  Q = tmp;
}

template <class T>
std::ostream& operator <<(std::ostream& output, const Vector2D<T>& v) {
  output << v[0] << " " << v[1];
  return output;
}

template <class T>
std::istream& operator >>(std::istream& input, Vector2D<T>& v) {
  input >> v[0] >> v[1];
  return input;
}

typedef Vector2D<float> Vector2Df;
typedef Vector2D<double> Vector2Dd;
typedef Vector2D<int> Vector2Di;

}

#endif /* SURFACE3D_VECTOR2D_H_ */
