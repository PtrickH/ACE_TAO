// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_codegen.cpp:151

#ifndef _TAO_IDL_ORIG_TIMEBASEC_H_
#define _TAO_IDL_ORIG_TIMEBASEC_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"
#include "tao/Any_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_Export.h"
#include "tao/SmartProxies/Smart_Proxies.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_Export

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO TAO_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option push -w-rvl -w-rch -w-ccc -w-inl
#endif /* __BORLANDC__ */

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_module/module_ch.cpp:48

TAO_NAMESPACE  TimeBase
{
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typedef/typedef_ch.cpp:334
  
  typedef CORBA::ULongLong TimeT;
  typedef CORBA::ULongLong_out TimeT_out;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_TimeT;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typedef/typedef_ch.cpp:334
  
  typedef TimeT InaccuracyT;
  typedef TimeT_out InaccuracyT_out;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_InaccuracyT;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typedef/typedef_ch.cpp:334
  
  typedef CORBA::Short TdfT;
  typedef CORBA::Short_out TdfT_out;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_TdfT;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_structure/structure_ch.cpp:49
  
  class UtcT_var;
  
  struct TAO_Export UtcT
  {
    typedef UtcT_var _var_type;
    
    static void _tao_any_destructor (void *);
    TimeBase::TimeT time;
    CORBA::ULong inacclo;
    CORBA::UShort inacchi;
    TimeBase::TdfT tdf;
  };
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_structure.cpp:71
  
  class TAO_Export UtcT_var
  {
  public:
    UtcT_var (void);
    UtcT_var (UtcT *);
    UtcT_var (const UtcT_var &);
    // Fixed-size types only.
    UtcT_var (const UtcT &);
    ~UtcT_var (void);
    
    UtcT_var &operator= (UtcT *);
    UtcT_var &operator= (const UtcT_var &);
    
    // Fixed-size types only.
    UtcT_var &operator= (const UtcT &);
    
    UtcT *operator-> (void);
    const UtcT *operator-> (void) const;
    
    operator const UtcT &() const;
    operator UtcT &();
    operator UtcT &() const;
    
    // in, inout, out, _retn 
    const UtcT &in (void) const;
    UtcT &inout (void);
    UtcT &out (void);
    UtcT _retn (void);
    UtcT *ptr (void) const;
  
  private:
    UtcT *ptr_;
  };
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_structure/structure_ch.cpp:105
  
  typedef UtcT &UtcT_out;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_UtcT;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_structure/structure_ch.cpp:49
  
  class IntervalT_var;
  
  struct TAO_Export IntervalT
  {
    typedef IntervalT_var _var_type;
    
    static void _tao_any_destructor (void *);
    TimeBase::TimeT lower_bound;
    TimeBase::TimeT upper_bound;
  };
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_structure.cpp:71
  
  class TAO_Export IntervalT_var
  {
  public:
    IntervalT_var (void);
    IntervalT_var (IntervalT *);
    IntervalT_var (const IntervalT_var &);
    // Fixed-size types only.
    IntervalT_var (const IntervalT &);
    ~IntervalT_var (void);
    
    IntervalT_var &operator= (IntervalT *);
    IntervalT_var &operator= (const IntervalT_var &);
    
    // Fixed-size types only.
    IntervalT_var &operator= (const IntervalT &);
    
    IntervalT *operator-> (void);
    const IntervalT *operator-> (void) const;
    
    operator const IntervalT &() const;
    operator IntervalT &();
    operator IntervalT &() const;
    
    // in, inout, out, _retn 
    const IntervalT &in (void) const;
    IntervalT &inout (void);
    IntervalT &out (void);
    IntervalT _retn (void);
    IntervalT *ptr (void) const;
  
  private:
    IntervalT *ptr_;
  };
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_structure/structure_ch.cpp:105
  
  typedef IntervalT &IntervalT_out;
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_IntervalT;

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_module/module_ch.cpp:67

}
TAO_NAMESPACE_CLOSE // module TimeBase

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_structure/any_op_ch.cpp:52

TAO_Export void operator<<= (CORBA::Any &, const TimeBase::UtcT &); // copying version
TAO_Export void operator<<= (CORBA::Any &, TimeBase::UtcT*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, TimeBase::UtcT *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const TimeBase::UtcT *&);

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_structure/any_op_ch.cpp:52

TAO_Export void operator<<= (CORBA::Any &, const TimeBase::IntervalT &); // copying version
TAO_Export void operator<<= (CORBA::Any &, TimeBase::IntervalT*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, TimeBase::IntervalT *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const TimeBase::IntervalT *&);

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_root/cdr_op.cpp:48

#ifndef __ACE_INLINE__

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_structure/cdr_op_ch.cpp:54

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const TimeBase::UtcT &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, TimeBase::UtcT &);

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_structure/cdr_op_ch.cpp:54

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const TimeBase::IntervalT &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, TimeBase::IntervalT &);

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_root/cdr_op.cpp:64

#endif /* __ACE_INLINE__ */

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_codegen.cpp:1009

#if defined (__ACE_INLINE__)
#include "TimeBaseC.i"
#endif /* defined INLINE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option pop
#endif /* __BORLANDC__ */

#include "ace/post.h"
#endif /* ifndef */

