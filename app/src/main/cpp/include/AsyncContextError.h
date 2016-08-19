/*
 * AsyncContextError.h
 *
 *  Created on: Mar 26, 2015
 *      Author: maxa
 */

#ifndef ASYNCCONTEXTERROR_H_
#define ASYNCCONTEXTERROR_H_

enum class AsyncContextError : int {
  ACONTEXT_OK = 0,
  ACONTEXT_WINDOW_NOT_SET = 1,
  ACONTEXT_NO_MESHES = 2,
  ACONTEXT_SCENE_TOO_LARGE = 3
};

#endif /* ASYNCCONTEXTERROR_H_ */
