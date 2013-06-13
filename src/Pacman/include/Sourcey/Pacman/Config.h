//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SOURCEY_Pacman_Config_H
#define SOURCEY_Pacman_Config_H


namespace Scy { 
namespace Pacman {

	
#define DEFAULT_API_ENDPOINT "http://localhost:3000"
#define DEFAULT_API_INDEX_URI "/packages.json"
#define DEFAULT_PACKAGE_CACHE_DIR "pacman/cache"
#define DEFAULT_PACKAGE_INTERMEDIATE_DIR "pacman/data"
#define DEFAULT_PACKAGE_INSTALL_DIR "pacman/install"
#define DEFAULT_PLATFORM "win32"


} } // namespace Scy::Pacman


#endif // SOURCEY_Pacman_Config_H