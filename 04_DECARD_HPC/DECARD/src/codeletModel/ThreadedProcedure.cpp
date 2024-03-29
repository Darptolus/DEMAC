/* 
 * Copyright (c) 2011-2014, University of Delaware
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "ThreadedProcedure.hpp"

using namespace decard;

ThreadedProcedure::ThreadedProcedure(void):
ref_(1), cdn_exec(0), cd_id(1)
{}//,
// parentTP_(myThread.tempParent){ } //*****COMMENTED*****//

//This is for paraFor loop
//ThreadedProcedure::ThreadedProcedure(unsigned int num, ThreadedProcedure * parentTP):
//ref_(num),
//parentTP_(parentTP){ }

ThreadedProcedure::~ThreadedProcedure(void)
{
  //We are deleting the TP
  //Does the TP have a parent
  if(parentTP_)
  {
    //Make sure he isn't a serial loop by checking if the parent has a parent
    if(parentTP_->checkParent())
    {
      //If he does lets dec the parent's counter and see if the parent has no more outstanding objects
      if(parentTP_->decRef())
      {
        delete parentTP_;
      }
    }
  }
  this->clear_all();
}

bool 
ThreadedProcedure::decRef(void)
{
    return (1==Atomics::fetchSub(ref_, 1U));
}

void 
ThreadedProcedure::incRef(void)
{
    Atomics::fetchAdd(ref_, 1U);
}

bool
ThreadedProcedure::zeroRef(void)
{
    return (!ref_);
}

bool 
ThreadedProcedure::checkParent(void)
{
    if(parentTP_==NULL)
        return false;
    if(parentTP_==this)
        return false;
    return true;
    //return (parentTP_!=this);
}

int
ThreadedProcedure::dec_cdnexec()
{ 
  return (Atomics::fetchSub(cdn_exec, 1));
}
//*****COMMENTED*****//
// void 
// ThreadedProcedure::add(Codelet * toAdd)
// {
//     //check on where to set
//     if(toAdd->codeletReady())
//     {
//         Atomics::fetchAdd(ref_, 1U);
//         myThread.threadTPsched->pushCodelet(toAdd);
//     }
// }