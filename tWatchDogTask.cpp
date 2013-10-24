//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//----------------------------------------------------------------------
/*!\file    rrlib/watchdog/tWatchDogTask.cpp
 *
 * \author  Max Reichardt
 *
 * \date    2012-05-08
 *
 */
//----------------------------------------------------------------------
#include "rrlib/watchdog/tWatchDogTask.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/watchdog/tWatchDog.h"

//----------------------------------------------------------------------
// Debugging
//----------------------------------------------------------------------
#include <cassert>

//----------------------------------------------------------------------
// Namespace usage
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Namespace declaration
//----------------------------------------------------------------------
namespace rrlib
{
namespace watchdog
{

//----------------------------------------------------------------------
// Forward declarations / typedefs / enums
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Const values
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// tWatchDogTask constructors
//----------------------------------------------------------------------
tWatchDogTask::tWatchDogTask(bool register_task) :
  dead_line(time::cNO_TIME),
  registered(register_task)
{
  if (register_task)
  {
    tWatchDog& i = tWatchDog::GetInstance();
    rrlib::thread::tLock lock(i.task_list_mutex);
    i.task_list.push_back(this);
  }
}

//----------------------------------------------------------------------
// tWatchDogTask destructor
//----------------------------------------------------------------------
tWatchDogTask::~tWatchDogTask()
{
  if (registered)
  {
    tWatchDog& i = tWatchDog::GetInstance();
    rrlib::thread::tLock lock(i.task_list_mutex);
    i.task_list.erase(std::remove(i.task_list.begin(), i.task_list.end(), this), i.task_list.end()); // Remove this task from list
  }
}

void tWatchDogTask::Deactivate()
{
  dead_line.Store(time::cNO_TIME);
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
