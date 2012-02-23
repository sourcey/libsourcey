//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_Pacman_Config_H
#define SOURCEY_Pacman_Config_H


namespace Sourcey { 
namespace Pacman {

	
#define DEFAULT_API_ENDPOINT "http://localhost:3000"
#define DEFAULT_API_INDEX_URI "/packages.json"
//#define DEFAULT_API_UPDATE_URI "/packages/list.json"
#define DEFAULT_PACKAGE_CACHE_DIR "packages/cache"
//#define DEFAULT_PACKAGE_MANIFEST_DIR "packages/manifest"
#define DEFAULT_PACKAGE_INTERMEDIATE_DIR "packages/data"
#define DEFAULT_PACKAGE_INSTALL_DIR ""
#define DEFAULT_PLATFORM "Win32"


} } // namespace Sourcey::Pacman


#endif // SOURCEY_Pacman_Config_H