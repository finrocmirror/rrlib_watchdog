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
#include "rrlib/time/time.h"

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

typedef rrlib::design_patterns::tSingletonHolder<tWatchDog, rrlib::design_patterns::singleton::Longevity> tWatchDogInstance;
static inline unsigned int GetLongevity(tWatchDog*)
{
  return 0xFCCCCCC0; // must be deleted before tThread thread local
}

tWatchDog& tWatchDog::GetInstance()
{
  return tWatchDogInstance::Instance();
}

void tWatchDog::Run()
{
  while (!IsStopSignalSet())
  {
    tWatchDogTask* stuck = NULL;

    // check tasks
    {
      time::tTimestamp cur_time = rrlib::time::Now();
      rrlib::thread::tLock l(task_list_mutex);
      for (std::vector<tWatchDogTask*>::iterator it = task_list.begin(); it < task_list.end(); it++)
      {
        time::tTimestamp dead_line = (*it)->GetDeadLine();
        if (dead_line != rrlib::time::cNO_TIME)
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
    else if (IsStopSignalSet())
    {
      break;
    }
    else
    {
      // wait (TODO: currently 1 second hard-coded)
      tThread::Sleep(std::chrono::seconds(1), false);
    }
  }
}

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}
