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

#include "Atomics.h"

namespace decard
{
  //This is a forward declaration since there is a circular dependence
  class Codelet;
  // class Node;

  class ThreadedProcedure 
  {
  protected:
  /*
  * Variable: ref_
  * ref_ says when the TP is done
  */
    // Node * origin_node;
    // Node * destin_node;
    int origin_node_id;
    int destin_node_id;
    int opr_id;
    unsigned int ref_;
  public:
    ThreadedProcedure * parentTP_;
    ThreadedProcedure(void);
    //ThreadedProcedure(unsigned int num, ThreadedProcedure * parentTP);
    //Destructor
    virtual ~ThreadedProcedure();
    
    bool decRef (void);
    void incRef (void);
    bool zeroRef (void);
    bool checkParent(void);

    // void set_orig(Node * a_o_node){
    //   this->origin_node = a_o_node;
    //   this->origin_node_id = a_o_node->get_id();
    // };
    // void set_dest(Node * a_d_node){
    //   this->destin_node = a_d_node;
    //   this->destin_node_id = a_d_node->get_id();
    // };
    void set_orig(int a_o_node){
      this->origin_node_id = a_o_node;
    };
    void set_dest(int a_d_node){
      this->destin_node_id = a_d_node;
    };
    void set_opr(int a_opr){
      this->opr_id = a_opr;
    };
    // Node * get_orig(){ return origin_node;};
    // Node * get_dest(){ return destin_node;};
    int get_orig_id(){ return origin_node_id;};
    int get_dest_id(){ return destin_node_id;};
    int * get_opr(){ return &opr_id;};
    /*
    * Method: addCodelet
    * Adds a codelet to the TP's list
    */
    // void add (Codelet * toAdd); //*****COMMENTED*****//
      
  };
}