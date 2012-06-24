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
/*!\file    rrlib/watchdog/tWatchDogTask.h
 *
 * \author  Max Reichardt
 *
 * \date    2012-05-08
 *
 * \brief   Contains tWatchDogTask
 *
 * \b tWatchDogTask
 *
 * Task for the watchdog.
 * If a deadline for such a task is exceeded, the watchdog reports this and executes a custom action.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__watchdog__tWatchDogTask_h__
#define __rrlib__watchdog__tWatchDogTask_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/time/tAtomicTimestamp.h"

//----------------------------------------------------------------------
// Internal includes with ""
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
// Class declaration
//----------------------------------------------------------------------
//! Task for the watchdog
/*!
 * Task for the watchdog.
 * If a deadline for such a task is exceeded, the watchdog reports this and executes a custom action.
 */
class tWatchDogTask
{
  friend class tWatchDog;

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*!
   * \param register_task Register task? (If false, task will not be checked. This is possibly useful for deactivating tasks in release mode.)
   */
  tWatchDogTask(bool register_task = true);

  virtual ~tWatchDogTask();

  /*!
   * Deactivates task checking by watchdog. It can be enabled by calling SetDeadLine().
   */
  void Deactivate();

  /*!
   * \return Current deadline for this task. cTASK_DEACTIVED if it is inactive.
   */
  time::tTimestamp GetDeadLine() const
  {
    return dead_line.Load();
  }

  /*!
   * \param dead_line New deadline for this task.
   */
  void SetDeadLine(const time::tTimestamp& dead_line)
  {
    this->dead_line.Store(dead_line);
  }

//----------------------------------------------------------------------
// Protected methods
//----------------------------------------------------------------------
protected:

  /*!
   * Needs to be overridden by subclass.
   *
   * MUST NOT GET STUCK in a dead-lock. Otherwise watch dog would get stuck, too.
   */
  virtual void HandleWatchdogAlert() = 0;

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  /*! Current task dead_line */
  rrlib::time::tAtomicTimestamp dead_line;

  /*! Has task been registered at watch dog thread? */
  bool registered;
};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}


#endif
