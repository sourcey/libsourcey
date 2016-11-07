///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/interface.h"
#include "scy/logger.h"
#include <memory>


namespace scy {


//
// Polymorphic
//


LogStream& basic::Polymorphic::log(const char* level) const
{
    return printL(level, className(), this);
}


} // namespace scy

/// @\}
