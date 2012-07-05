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
/*!\file    rrlib/watchdog/tWatchDog.h
 *
 * \author  Max Reichardt
 *
 * \date    2012-05-08
 *
 * \brief   Contains tWatchDog
 *
 * \b tWatchDog
 *
 * Realizes a watchdog: Watch tasks can be assigned to this class/thread.
 * If they not complete or send a keep-alive signal until a specified dead-line,
 * the watchdog will notify the user and execute a custom action.
 *
 */
//----------------------------------------------------------------------
#ifndef __rrlib__watchdog__tWatchDog_h__
#define __rrlib__watchdog__tWatchDog_h__

//----------------------------------------------------------------------
// External includes (system with <>, local with "")
//----------------------------------------------------------------------
#include "rrlib/thread/tThread.h"
#include "rrlib/util/patterns/singleton.h"

//----------------------------------------------------------------------
// Internal includes with ""
//----------------------------------------------------------------------
#include "rrlib/watchdog/tWatchDogTask.h"

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
//! Realizes a watchdog
/*!
 * Realizes a watchdog: Watch tasks can be assigned to this class/thread.
 * If they not complete or send a keep-alive signal until a specified dead-line,
 * the watchdog will notify the user and execute a custom action.
 */
class tWatchDog : public rrlib::thread::tThread
{
  friend class tWatchDogTask;
  friend class rrlib::util::singleton::CreateUsingNew<tWatchDog>;

//----------------------------------------------------------------------
// Public methods and typedefs
//----------------------------------------------------------------------
public:

  /*!
   * \return Watch dog singleton instance
   */
  static tWatchDog& GetInstance();

//----------------------------------------------------------------------
// Protected methods
//----------------------------------------------------------------------
protected:

  virtual void Run();

  virtual void StopThread();

//----------------------------------------------------------------------
// Private fields and methods
//----------------------------------------------------------------------
private:

  tWatchDog();
  virtual ~tWatchDog();

  /*! Task list for watch dog. tWatchDogTask class adds task in its constructor - and removes it in destructor. */
  std::vector<tWatchDogTask*> task_list;

  /*! Mutex for task list */
  rrlib::thread::tMutex task_list_mutex;
};

//----------------------------------------------------------------------
// End of namespace declaration
//----------------------------------------------------------------------
}
}


#endif
