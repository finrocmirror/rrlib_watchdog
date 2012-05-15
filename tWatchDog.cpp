//
// You received this file as part of RRLib
// Robotics Research Library
//
// Copyright (C) Finroc GbR (finroc.org)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//----------------------------------------------------------------------
/*!\file    rrlib/watchdog/tWatchDog.cpp
 *
 * \author  Max Reichardt
 *
 * \date    2012-05-08
 *
 */
//----------------------------------------------------------------------
#include "rrlib/watchdog/tWatchDog.h"

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/finroc_core_utils/tTime.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------

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
// tWatchDog constructors
//----------------------------------------------------------------------
tWatchDog::tWatchDog() :
  tThread("Watchdog")
{
  Start();
}

//----------------------------------------------------------------------
// tWatchDog destructor
//----------------------------------------------------------------------
tWatchDog::~tWatchDog()
{
  StopThread();
  Join();
}

typedef rrlib::util::tSingletonHolder<tWatchDog, rrlib::util::singleton::Longevity> tWatchDogInstance;
static inline unsigned int GetLongevity(tWatchDog*)
{
  return 0xFFFFFFFE; // can be deleted early
}

tWatchDog& tWatchDog::GetInstance()
{
  return tWatchDogInstance::Instance();
}

void tWatchDog::Run()
{
  while (!stop_signal)
  {
    tWatchDogTask* stuck = NULL;

    // check tasks
    {
      int64_t cur_time = finroc::util::tTime::GetPrecise();
      finroc::util::tLock l(task_list_mutex);
      for (std::vector<tWatchDogTask*>::iterator it = task_list.begin(); it < task_list.end(); it++)
      {
        int64_t dead_line = (*it)->GetDeadLine();
        if (dead_line != tWatchDogTask::cTASK_DEACTIVED)
        {
          if (cur_time > dead_line)
          {
            stuck = *it;
            break;
          }
        }
      }
    }

    if (stuck)
    {
      stuck->HandleWatchdogAlert(); // Do this outside of list lock to avoid deadlocks from deleting tasks.
    }
    else if (stop_signal)
    {
      break;
    }
    else
    {
      // wait (TODO: currently 1 second hard-coded)
      tThread::Sleep(1000);
    }
  }
}

void tWatchDog::StopThread()
{
  stop_signal = true;
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
