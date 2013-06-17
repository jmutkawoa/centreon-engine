/*
** Copyright 2011-2013 Merethis
**
** This file is part of Centreon Engine.
**
** Centreon Engine is free software: you can redistribute it and/or
** modify it under the terms of the GNU General Public License version 2
** as published by the Free Software Foundation.
**
** Centreon Engine is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Centreon Engine. If not, see
** <http://www.gnu.org/licenses/>.
*/

#include <cstring>
#include "com/centreon/engine/misc/string.hh"
#include "com/centreon/engine/objects/timeperiodexclusion.hh"

using namespace com::centreon::engine::misc;

/**
 *  Equal operator.
 *
 *  @param[in] obj1 The first object to compare.
 *  @param[in] obj2 The second object to compare.
 *
 *  @return True if is the same object, otherwise false.
 */
bool operator==(
       timeperiodexclusion const& obj1,
       timeperiodexclusion const& obj2) throw () {
  return (!strcmp(obj1.timeperiod_name, obj2.timeperiod_name));
}

/**
 *  Not equal operator.
 *
 *  @param[in] obj1 The first object to compare.
 *  @param[in] obj2 The second object to compare.
 *
 *  @return True if is not the same object, otherwise false.
 */
bool operator!=(
       timeperiodexclusion const& obj1,
       timeperiodexclusion const& obj2) throw () {
  return (!operator==(obj1, obj2));
}

/**
 *  Dump timeperiodexclusion content into the stream.
 *
 *  @param[out] os  The output stream.
 *  @param[in]  obj The timeperiodexclusion to dump.
 *
 *  @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, timeperiodexclusion const& obj) {
  for (timeperiodexclusion const* m(&obj); m; m = m->next)
    os << chkstr(m->timeperiod_name) << (m->next ? ", " : "");
  return (os);
}

