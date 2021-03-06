// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    OrbTask.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef ORB_TASK_H
#define ORB_TASK_H

#include "CSD_PT_TestInf_Export.h"
#include "ace/Task.h"
#include "tao/ORB.h"


class CSD_PT_TestInf_Export OrbTask : public ACE_Task_Base
{
  public:

    OrbTask(CORBA::ORB_ptr orb, unsigned num_threads = 1);
    virtual ~OrbTask();

    virtual int open(void*);
    virtual int svc();
    virtual int close(u_long);


  private:

    CORBA::ORB_var orb_;
    unsigned       num_threads_;
};

#endif
