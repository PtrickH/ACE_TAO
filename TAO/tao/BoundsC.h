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

#ifndef _TAO_IDL_ORIG_BOUNDSC_H_
#define _TAO_IDL_ORIG_BOUNDSC_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_Export.h"
#include "Exception.h"
#include "CDR.h"

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

TAO_NAMESPACE  CORBA
{
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_exception/exception_ch.cpp:51

#if !defined (_CORBA_BOUNDS_CH_)
#define _CORBA_BOUNDS_CH_
  
  class TAO_Export Bounds : public CORBA::UserException
  {
  public:
    
    Bounds (void);
    Bounds (const Bounds &);
    ~Bounds (void);

    Bounds &operator= (const Bounds &);
    
    static void _tao_any_destructor (void *);
    
    static Bounds *_downcast (CORBA::Exception *);
    static CORBA::Exception *_alloc (void);

    virtual CORBA::Exception *_tao_duplicate (void) const;

    virtual void _raise (void);

    virtual void _tao_encode (
        TAO_OutputCDR &
        ACE_ENV_ARG_DECL_NOT_USED
      ) const;
    
    virtual void _tao_decode (
        TAO_InputCDR &
        ACE_ENV_ARG_DECL_NOT_USED
      );
    
    // TAO_IDL - Generated from
    // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_exception/exception_ch.cpp:125
    
    virtual CORBA::TypeCode_ptr _type (void) const;
  };
  
  // TAO_IDL - Generated from
  // W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_Bounds;

#endif /* end #if !defined */

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_module/module_ch.cpp:67

}
TAO_NAMESPACE_CLOSE // module CORBA

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_exception/any_op_ch.cpp:52

TAO_Export void operator<<= (CORBA::Any &, const CORBA::Bounds &); // copying version
TAO_Export void operator<<= (CORBA::Any &, CORBA::Bounds*); // noncopying version
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, CORBA::Bounds *&); // deprecated
TAO_Export CORBA::Boolean operator>>= (const CORBA::Any &, const CORBA::Bounds *&);

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_root/cdr_op.cpp:48

#ifndef __ACE_INLINE__

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_exception/cdr_op_ch.cpp:52

TAO_Export CORBA::Boolean operator<< (TAO_OutputCDR &, const CORBA::Bounds &);
TAO_Export CORBA::Boolean operator>> (TAO_InputCDR &, CORBA::Bounds &);

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_visitor_root/cdr_op.cpp:64

#endif /* __ACE_INLINE__ */

// TAO_IDL - Generated from
// W:\ACE_wrappers\TAO\TAO_IDL\be\be_codegen.cpp:1009

#if defined (__ACE_INLINE__)
#include "BoundsC.i"
#endif /* defined INLINE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option pop
#endif /* __BORLANDC__ */

#include "ace/post.h"
#endif /* ifndef */

