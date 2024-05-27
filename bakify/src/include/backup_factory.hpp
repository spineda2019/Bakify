/*
 *  main.cpp - CLI setup and pass along to thread pool
 *  Copyright (C) 2024  Sebastian Pineda (spineda.wpi.alum@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>
 */

#ifndef BAKIFY_SRC_INCLUDE_BACKIFY_FACTORY_HPP_
#define BAKIFY_SRC_INCLUDE_BACKIFY_FACTORY_HPP_

#include <string>
#include <vector>

void StartBackingUp(std::vector<std::string>&& file_names);

#endif  // BAKIFY_SRC_INCLUDE_BACKIFY_FACTORY_HPP_
