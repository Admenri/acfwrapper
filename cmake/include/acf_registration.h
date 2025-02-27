// Copyright (c) 2023-2024 Admenri.
// All rights reserved.

#ifndef ACF_INCLUDE_ACF_REGISTRATION_H_
#define ACF_INCLUDE_ACF_REGISTRATION_H_

#include "include/internal/acf_scoped_refptr.h"

///
/// Generic callback interface used for managing the lifespan of a registration.
///
/*--acf(source=library)--*/
class AcfRegistration : public virtual AcfBaseRefCounted {};

#endif  // ACF_INCLUDE_REGISTRATION_H_