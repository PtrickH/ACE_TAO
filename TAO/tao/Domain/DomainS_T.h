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

#ifndef _TAO_IDL_DOMAINS_T_H_
#define _TAO_IDL_DOMAINS_T_H_

#include "ace/pre.h"
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined (ACE_HAS_USING_KEYWORD)

  // TIE class: Refer to CORBA v2.2, Section 20.34.4
  template <class T>
  class TAO_Domain_Export POA_CORBA_DomainManager_tie : public POA_CORBA_DomainManager
  {
  public:
    POA_CORBA_DomainManager_tie (T &t);
    // the T& ctor
    POA_CORBA_DomainManager_tie (T &t, PortableServer::POA_ptr poa);
    // ctor taking a POA
    POA_CORBA_DomainManager_tie (T *tp, CORBA::Boolean release=1);
    // ctor taking pointer and an ownership flag
    POA_CORBA_DomainManager_tie (T *tp, PortableServer::POA_ptr poa, CORBA::Boolean release=1);
    // ctor with T*, ownership flag and a POA
    ~POA_CORBA_DomainManager_tie (void);
    // dtor

    // TIE specific functions
    T *_tied_object (void);
    // return the underlying object
    void _tied_object (T &obj);
    // set the underlying object
    void _tied_object (T *obj, CORBA::Boolean release=1);
    // set the underlying object and the ownership flag
    CORBA::Boolean _is_owner (void);
    // do we own it
    void _is_owner (CORBA::Boolean b);
    // set the ownership

    // overridden ServantBase operations
    PortableServer::POA_ptr _default_POA (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );
    CORBA::Policy_ptr get_domain_policy (
        CORBA::PolicyType policy_type,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  private:
    T *ptr_;
    PortableServer::POA_var poa_;
    CORBA::Boolean rel_;

    // copy and assignment are not allowed
    POA_CORBA_DomainManager_tie (const POA_CORBA_DomainManager_tie &);
    void operator= (const POA_CORBA_DomainManager_tie &);
  };

  // TIE class: Refer to CORBA v2.2, Section 20.34.4
  template <class T>
  class TAO_Export POA_CORBA_ConstructionPolicy_tie : public POA_CORBA_ConstructionPolicy
  {
  public:
    POA_CORBA_ConstructionPolicy_tie (T &t);
    // the T& ctor
    POA_CORBA_ConstructionPolicy_tie (T &t, PortableServer::POA_ptr poa);
    // ctor taking a POA
    POA_CORBA_ConstructionPolicy_tie (T *tp, CORBA::Boolean release=1);
    // ctor taking pointer and an ownership flag
    POA_CORBA_ConstructionPolicy_tie (T *tp, PortableServer::POA_ptr poa, CORBA::Boolean release=1);
    // ctor with T*, ownership flag and a POA
    ~POA_CORBA_ConstructionPolicy_tie (void);
    // dtor

    // TIE specific functions
    T *_tied_object (void);
    // return the underlying object
    void _tied_object (T &obj);
    // set the underlying object
    void _tied_object (T *obj, CORBA::Boolean release=1);
    // set the underlying object and the ownership flag
    CORBA::Boolean _is_owner (void);
    // do we own it
    void _is_owner (CORBA::Boolean b);
    // set the ownership

    // overridden ServantBase operations
    PortableServer::POA_ptr _default_POA (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      );
    void make_domain_manager (
        CORBA::InterfaceDef_ptr object_type,
        CORBA::Boolean constr_policy,
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    CORBA::PolicyType policy_type (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    CORBA::Policy_ptr copy (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

    void destroy (
        CORBA::Environment &ACE_TRY_ENV =
          TAO_default_environment ()
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  private:
    T *ptr_;
    PortableServer::POA_var poa_;
    CORBA::Boolean rel_;

    // copy and assignment are not allowed
    POA_CORBA_ConstructionPolicy_tie (const POA_CORBA_ConstructionPolicy_tie &);
    void operator= (const POA_CORBA_ConstructionPolicy_tie &);
  };


#endif /* ACE_HAS_USING_KEYWORD */

#if defined (__ACE_INLINE__)
#include "DomainS_T.i"
#endif /* defined INLINE */


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "DomainS_T.cpp"
#endif /* defined REQUIRED SOURCE */


#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("DomainS_T.cpp")
#endif /* defined REQUIRED PRAGMA */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* ifndef */
