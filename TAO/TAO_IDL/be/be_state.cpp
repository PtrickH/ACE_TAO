// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state.cpp
//
// = DESCRIPTION
//    state based code generation.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// constructor
be_state::be_state (void)
{
}

// destructor
be_state::~be_state (void)
{
}

// constructor
be_state_attribute::be_state_attribute (void)
{
}

int
be_state_attribute::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (bt);
  ACE_UNUSED_ARG (d);
  ACE_UNUSED_ARG (type);

  return 0;
}

// ==== all subclasses of be_state ======

be_state_struct_ch::be_state_struct_ch (void)
{
}

// generate code for structure member
int
be_state_struct_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_field *f;       // field node
  be_structure *bs;  // enclosing structure node

  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (nl);

  os = cg->client_header (); // get client header stream
  f = be_field::narrow_from_decl (d); // downcast to field node
  if (!f)
    return -1;

  bs = be_structure::narrow_from_scope (f->defined_in ());
  if (bs == NULL)
    return -1;

  // pass the field node just incase it is needed
  cg->node (f);

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        os->indent (); // start from current indentation
        *os << bt->nested_type_name (bs, "_var") << " " << f->local_name () <<
          ";\n\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        os->indent (); // start from current indentation
        *os << bt->nested_type_name (bs) << " " << f->local_name () << ";\n\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bt->nested_type_name (bs, "_var") << " " << f->local_name () << ";\n\n";
          }
        else
          {
            *os << "CORBA::String_var " << f->local_name () << ";\n\n";
          }
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        // We first need to generate code for this aggregate type. Check if we
        // are not called recursively thru a typedef
        if (bt->node_type () != AST_Decl::NT_typedef)
          if (bt->gen_client_header () == -1)
            return -1;

        os->indent ();
        *os << bt->nested_type_name (bs) << " " << f->local_name () << ";\n\n";
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, f, temp);
      } // end of switch
      break;
    }
  // the enclosing structure will be variable length the field is variable
  // length
  bs->size_type (type->size_type ());
  return 0;
}

be_state_union_disctypedefn_ch::be_state_union_disctypedefn_ch (void)
{
}

// generate code for union discriminant type in client header
int
be_state_union_disctypedefn_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union *bu;

  bu = be_union::narrow_from_decl (d); // downcast to union type
  if (!bu)
    return -1;

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  os = cg->client_header (); // get client header stream

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_string: // type is a string
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
    case AST_Decl::NT_except: // type is an exception
      // all these cases are syntax errors which the front-end should have
      // flagged as error cases
      return -1;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
      {
        // if the discriminant is an enum, first generate the enum
        // definition. However, check that we are not inside a recursive call
        if (bt->node_type () == AST_Decl::NT_enum)
          if (bt->gen_client_header () == -1)
            return -1;

        os->indent ();
        // the set method
        *os << "void _d (" << bt->nested_type_name (bu) << ");" << nl;
        // the get method
        *os << bt->nested_type_name (bu) << " _d (void) const;\n\n";
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        // a scoped name must result in one of the allowed types. Hopefully the
        // front-end has done a good job of ensuring this.
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);
        if (!t)
          return -1;
        temp = t->primitive_base_type ();
        return this->gen_code (t, d, temp);
      }
      break;
    } // end of switch
  return 0;
}

be_state_union_disctypedefn_ci::be_state_union_disctypedefn_ci (void)
{
}

// generate code for union discriminant type in client inline
int
be_state_union_disctypedefn_ci::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union *bu;

  bu = be_union::narrow_from_decl (d); // downcast to union type
  if (!bu)
    return -1;

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  os = cg->client_inline (); // get client inline stream

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_string: // type is a string
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
    case AST_Decl::NT_except: // type is an exception
      // all these cases are syntax errors which the front-end should have
      // flagged as error
      return -1;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_typedef: // type is a typedef
      {
        os->indent ();
        // the set method
        *os << "// accessor to set the discriminant" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::_d (" << bt->name () <<
          " discval)" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "this->disc_ = discval;\n";
        os->decr_indent ();
        *os << "}" << nl;
        // the get method
        *os << "// accessor to get the discriminant" << nl;
        *os << "ACE_INLINE " << bt->name () << nl;
        *os << bu->name () << "::_d (void) const" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->disc_;\n";
        os->decr_indent ();
        *os << "}\n\n";
      }
      break;
    } // end of switch
  return 0;
}

be_state_union_public_ch::be_state_union_public_ch (void)
{
}

// generate code for union branch members in client header.  This involves
// generating the set/get methods corresponding to the members
int
be_state_union_public_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union_branch *ub; // union branch member
  be_union *bu;      // enclosing union

  os = cg->client_header (); // get client header stream
  ub = be_union_branch::narrow_from_decl (d); // downcast to union branch node
  if (!ub)
    return -1;

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (bu == NULL)
    return -1;

  // pass the union branch node just incase it is needed
  cg->node (ub);

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        os->indent (); // start from current indentation
        *os << "void " << ub->local_name () << " (" << bt->nested_type_name
          (bu, "_ptr") << ");// set" << nl;
        *os << bt->nested_type_name (bu, "_ptr") << " " << ub->local_name () <<
          " (void) const; // get method\n\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
      {
        // if the type is an enum, we generate its defn first
        if (bt->node_type () == AST_Decl::NT_enum)
          if (bt->gen_client_header () == -1)
            return -1;

        os->indent (); // start from current indentation
        *os << "void " << ub->local_name () << " (" << bt->nested_type_name (bu)
          << ");// set" << nl;
        *os << bt->nested_type_name (bu) << " " << ub->local_name () <<
          " (void) const; // get method\n\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            // three methods to set the string value
            *os << "void " << ub->local_name () << " (" << bt->nested_type_name
              (bu) << "); // set" << nl;
            *os << "void " << ub->local_name () << " (const " <<
              bt->nested_type_name (bu) << "); // set"
                << nl;
            *os << "void " << ub->local_name () <<
              " (const " << bt->nested_type_name (bu, "_var") << " &); // set"  <<
              nl;
            *os << "const " << bt->nested_type_name (bu) << " " <<
              ub->local_name () << " (void) const; // get method\n\n";
          }
        else
          {
            // three methods to set the string value
            *os << "void " << ub->local_name () << " (char *); // set" << nl;
            *os << "void " << ub->local_name () << " (const char *); // set"
                << nl;
            *os << "void " << ub->local_name () <<
              " (const CORBA::String_var&); // set"  << nl;
            *os << "const char *" << ub->local_name () <<
              " (void) const; // get method\n\n";
          }
      }
      break;
    case AST_Decl::NT_array: // type is an array
      {
        // generate code for the array. So let the array handle code
        // generation. Check if this is not a recursive call
        if (bt->node_type () != AST_Decl::NT_typedef)
          if (bt->gen_client_header () == -1)
            return -1;

        os->indent ();
        *os << "void " << ub->local_name () << " (" << bt->nested_type_name
          (bu) << ");// set" << nl;
        *os << bt->nested_type_name (bu, "_slice") << " *" << ub->local_name () <<
          " (void) const; // get method\n\n";

      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        // generate defn for this aggregate unless we are recursively called
        if (bt->node_type () != AST_Decl::NT_typedef)
          if (bt->gen_client_header () == -1)
            return -1;
        os->indent ();
        *os << "void " << ub->local_name () << " (const " <<
          bt->nested_type_name (bu) << " &);// set" << nl;
        *os << "const " << bt->nested_type_name (bu) << " &" << ub->local_name
          () << " (void) const; // get method (read only)" << nl;
        *os << bt->nested_type_name (bu) << " &" << ub->local_name () <<
          " (void); // get method (read/write only)\n\n";

      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG: Is this case valid ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, ub, temp);
      }
      break;
    } // end of switch

  // enclosing union is variable if the member is variable
  bu->size_type (type->size_type ());

  return 0;
}

be_state_union_public_ci::be_state_union_public_ci (void)
{
}

// generate code for union branch members in client inline.  This involves
// generating the set/get methods corresponding to the members
int
be_state_union_public_ci::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os;   // output stream
  TAO_NL  nl;          // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union_branch *ub; // union branch member
  be_union *bu;        // enclosing union

  os = cg->client_inline (); // get client inline stream
  ub = be_union_branch::narrow_from_decl (d); // downcast to union branch node
  if (!ub)
    return -1;

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (bu == NULL)
    return -1;

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_array: // type is an array
      {
        // set method
        os->indent (); // start from current indentation
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (" << bt->name ();
        if (type->node_type () == AST_Decl::NT_interface)
          {
            // if it is an interface node, we use _ptr as a suffix for the type
            *os << "_ptr";
          }
        *os << " val)// set" << nl;
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // get method
        *os << "// retrieve the member" << nl;
        *os << "ACE_INLINE " << bt->name ();
        if (type->node_type () == AST_Decl::NT_interface)
          {
            // for interface type, return a _ptr type
            *os << "_ptr";
          }
        else if (type->node_type () == AST_Decl::NT_array)
          {
            // for an array, return a _slice* type
            *os << "_slice *";
          }
        *os << nl;
        *os << bu->name () << "::" << ub->local_name () <<
          " (void) const" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}\n\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        // three methods to set the string value

        // (1) set method from char*
        os->indent (); // start from current indentation
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bu->name () << "::" << ub->local_name () << " (" << bt->name
              () << " val)" << nl;
          }
        else
          {
            *os << bu->name () << "::" << ub->local_name () << " (char *val)"
                << nl;
          }
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // (2) set method from const char *
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bu->name () << "::" << ub->local_name () << " (const " <<
              bt->name () << " val)" << nl;
          }
        else
          {
            *os << bu->name () << "::" << ub->local_name () <<
              " (const char *val)" << nl;
          }
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // (3) set from const String_var&
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bu->name () << "::" << ub->local_name () << " (const " <<
              bt->name () << "_var &val)" << nl;
          }
        else
          {
            *os << bu->name () << "::" << ub->local_name () <<
              " (const CORBA::String_var &val)" << nl;
          }
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // return const char*
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << "ACE_INLINE " << bt->name () << nl;
          }
        else
          {
            *os << "ACE_INLINE const char *" << nl;
          }
        *os << bu->name () << "::" << ub->local_name () <<
          " (void) const // get method" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}\n\n";
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        os->indent ();

        // (1) set from a const
        *os << "// accessor to set the member" << nl;
        *os << "ACE_INLINE void" << nl;
        *os << bu->name () << "::" << ub->local_name () <<
          " (const " << bt->name () << " &val)" << nl;
        *os << "{\n";
        os->incr_indent ();
        // set the discriminant to the appropriate label
        if (ub->label ()->label_kind () == AST_UnionLabel::UL_label)
          {
            // valid label
            *os << "// set the discriminant val" << nl;
            // check if the case label is a symbol or a literal
            if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
              {
                *os << "this->disc_ = " << ub->label ()->label_val ()->n ()
                    << ";" << nl;
              }
            else
              {
                *os << "this->disc_ = " << ub->label ()->label_val () << ";" <<
                  nl;
              }
            *os << "// set the value" << nl;
            *os << "this->" << ub->local_name () << "_ = val;\n";
          }
        else
          {
            // default label
            // XXXASG - TODO
          }
        os->decr_indent ();
        *os << "}" << nl;

        // readonly get method
        *os << "// readonly get method " << nl;
        *os << "ACE_INLINE const " << bt->name () << " &" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (void) const" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}" << nl;

        // read/write get method
        *os << "// read/write get method " << nl;
        *os << "ACE_INLINE " << bt->name () << " &" << nl;
        *os << bu->name () << "::" << ub->local_name () << " (void)" << nl;
        *os << "{\n";
        os->incr_indent ();
        *os << "return this->" << ub->local_name () << "_;\n";
        os->decr_indent ();
        *os << "}" << nl;

      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG: Is this case valid ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, ub, temp);
      }
      break;
    } // end of switch

  return 0;
}

be_state_union_private_ch::be_state_union_private_ch (void)
{
}

int
be_state_union_private_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_union_branch *ub; // union branch member
  be_union *bu;      // enclosing union

  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (nl);

  os = cg->client_header (); // get client header stream
  ub = be_union_branch::narrow_from_decl (d); // downcast to union branch node
  if (!ub)
    return -1;

  bu = be_union::narrow_from_scope (ub->defined_in ());
  if (bu == NULL)
    return -1;

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        os->indent (); // start from current indentation
        *os << bt->nested_type_name (bu, "_var") << " " << ub->local_name () <<
          "_;\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
      {
        os->indent (); // start from current indentation
        *os << bt->nested_type_name (bu) << " " << ub->local_name () <<
          "_;\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bt->nested_type_name (bu, "_var") << " " << ub->local_name () <<
              "_;\n";
          }
        else
          {
            *os << "CORBA::String_var " << ub->local_name () << "_;\n";
          }
      }
      break;
    case AST_Decl::NT_array: // type is an array
      {
        os->indent ();
        *os << bt->nested_type_name (bu, "_slice") << " *" << ub->local_name () <<
          "_;\n";
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        os->indent ();
        *os << bt->nested_type_name (bu) << " " << ub->local_name () << "_;\n";
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG: Is this case valid ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, ub, temp);
      }
      break;
    } // end of switch
  return 0;
}

// return type for operation
be_state_operation::be_state_operation (void)
{
}

// generate code for return type of operation
int
be_state_operation::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_operation *bop;
  be_interface *bif; // interface in which the operation was defined

  bop = be_operation::narrow_from_decl (d);
  if (!bop)
    return -1;

  bif = be_interface::narrow_from_scope (bop->defined_in ());
  if (!bif)
    return -1;

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_OPERATION_SH:
      os = cg->server_header ();
      break;
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
    case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
      os = cg->server_skeletons ();
      break;
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_interface_fwd: // type is an obj reference
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << "_ptr ";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              *os << "CORBA::Object_ptr retval = CORBA::Object::_nil ();" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return " << bt->name () << "::_nil ();" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return " << bt->name () << "::_narrow (retval);" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              //              *os << bt->name () << "_ptr retval;"; // callee
              //              allocates
              // some stupid problems arising out of casting to the bt->name type
              *os << "CORBA::Object_ptr retval;" << nl; // callee allocates
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "retval"; // assign to retval
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif, "_ptr") << " ";
            }
            break;
          default:
            {
              *os << bt->name () << "_ptr ";
            }
          }
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (bt);

        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << bt->name () << " *";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << bt->name () << "_ptr ";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_void)
                {
                  *os << "void ";
                }
              else
                {
                  *os << bt->name () << " ";
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << bt->name () << " *retval;" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << bt->name () << "_ptr retval;" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_void)
                {
                  // no return variable
                }
              else
                {
                  *os << bt->name () << " retval;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << "return 0;" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << "return 0;" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_void)
                {
                  *os << "return;" << nl;
                }
              else
                {
                  *os << "return retval;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              if (bpd->pt () == AST_PredefinedType::PT_void)
                {
                  *os << "return; // no value" << nl;
                }
              else
                {
                  *os << "return retval;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << bt->name () << " *retval;" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << bt->name () << "_ptr retval;" << nl;
                }
              else
                {
                  *os << bt->name () << " *retval = new " << bt->name () << ";"
                      << nl;
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << "retval";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << "retval";
                }
              else
                {
                  *os << " *retval";
                }
            }
            break;
          default:
            {
              *os << bt->name ();
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << " *";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << "_ptr";
                }
            }
          } // end switch (cg->state())
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name ();
                }
              else
                {
                  *os << "char *";
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name () << " retval = 0;" << nl;
                }
              else
                {
                  *os << "char *retval = 0;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name () << " retval;" << nl;
                }
              else
                {
                  *os << "char *retval;" << nl;
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "retval";
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  // to keep MSVC++ happy
                  *os << bt->nested_type_name (bif);
                }
              else
                {
                  *os << "char *";
                }
            }
            break;
          default:
            {
              *os << "char *";
            }
          } // end of switch cg->state
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << "_slice *";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              *os << bt->name () << "_slice *retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              *os << bt->name () << "_slice *retval;" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "retval";
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif, "_slice") << " *";
            }
            break;
          default:
            {
              // return a pointer to slice
              *os << bt->name () << "_slice *";
            }
          } // end of switch cg->state
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
      // return type is a pointer to sequence
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << " *";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              *os << bt->name () << " *retval = 0;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              *os << bt->name () << " *retval;" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "retval";
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif) << " *";
            }
            break;
          default:
            {
              *os << bt->name () << " *";
            }
          } // end of swithc cg->state
      }
      break;
    case AST_Decl::NT_enum: // type is an enum
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << " ";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              *os << bt->name () << " retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              *os << bt->name () << " *retval = new " << bt->name () << ";" <<
                nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "*retval";
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif);
            }
            break;
          default:
            {
              *os << bt->name ();
            }
          } // end of switch cg->state
      }
      break;
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << " ";
              if (bt->size_type () == be_decl::VARIABLE)
                {
                  *os << "*";
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              if (bt->size_type () == be_decl::VARIABLE)
                {
                  *os << bt->name () << " *retval;" << nl;
                }
              else
                {
                  *os << bt->name () << " retval;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              if (bt->size_type () == be_decl::VARIABLE)
                {
                  *os << "return 0;" << nl;
                }
              else
                {
                  *os << "return retval;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              if (type->size_type () == be_decl::VARIABLE)
                {
                  *os << bt->name () << " *retval;" << nl;
                }
              else
                {
                  *os << bt->name () << " *retval = new " << bt->name () << ";"
                      << nl;
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              if (type->size_type () == be_decl::VARIABLE)
                {
                  *os << "retval";
                }
              else
                {
                  *os << "*retval";
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif);
              // check if we are fixed size or variable sized. Depending on that we
              // return a pointer or the aggregate itself
              if (type->size_type () == be_decl::VARIABLE)
                *os << " *";
            }
            break;
          default:
            {
              *os << bt->name ();
              // check if we are fixed size or variable sized. Depending on that we
              // return a pointer or the aggregate itself
              if (type->size_type () == be_decl::VARIABLE)
                *os << " *";
            }
          } // end of switch cg->state
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      } // end of switch
      break;
    } // switch of main switch
  return 0;
}

// return type for argument
be_state_argument::be_state_argument (void)
{
}

// generate code for return type of argument
int
be_state_argument::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_argument *arg;  // argument node
  be_interface *bif; // interface inside which the operation that uses this
                     // argument was defined

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_ARGUMENT_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_SH:
      os = cg->server_header ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_SS:
    case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
      os = cg->server_skeletons ();
      break;
    }

  arg = be_argument::narrow_from_decl (d);
  if (!arg)
    return -1;

  // get the scope of the arg which is the operation. Its scope is the
  // interface node
  bif = be_interface::narrow_from_scope (ScopeAsDecl (arg->defined_in
                                                      ())->defined_in ());
  if (!bif)
    return -1;

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // find the direction of the argument. Depending on the direction and the
  // type, generate the mapping
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_interface_fwd: // type is an obj reference
      {
        switch (arg->direction ())
          {
          case AST_Argument::dir_IN:
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                *os << bt->name () << "_ptr ";
                // declare a variable
                *os << arg->local_name () << ";" << nl;
                // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", " << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif, "_ptr") << " ";
              }
            else
              {
                *os << bt->name () << "_ptr ";
              }
            break;
          case AST_Argument::dir_INOUT:
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                *os << bt->name () << "_ptr ";
                // declare a variable
                *os << arg->local_name () << ";" << nl;
                // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", " << arg->local_name () <<
                  ", 1); // ORB owns" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif, "_ptr") << " &";
              }
            else
              {
                *os << bt->name () << "_ptr &";
              }
            break;
          case AST_Argument::dir_OUT:
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                // declare a variable
                *os << bt->name () << "_ptr ";
                *os << arg->local_name () << ";" << nl;
                // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", " << arg->local_name () <<
                  ", 1); // ORB owns" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif, "_out") << " ";
              }
            else
              {
                *os << bt->name () << "_out ";
              }
            break;
          }
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (bt);

        // check if the type is an any
        if (bpd->pt () == AST_PredefinedType::PT_any)
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    *os << bt->name () << " ";
                    // declare a variable
                    *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", &" << arg->local_name () <<
                      "); // ORB does not own" << nl;
                  }
                else
                  {
                    *os << "const " << bt->name () << " &";
                  }
                break;
              case AST_Argument::dir_INOUT:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    // declare a variable
                    *os << bt->name () << " ";
                    *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", &" << arg->local_name () <<
                      ", 1); // ORB owns" << nl;
                  }
                else
                  {
                    *os << bt->name () << " &";
                  }
                break;
              case AST_Argument::dir_OUT:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    // declare a variable
                    *os << bt->name () << " *";
                    *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", " << arg->local_name () <<
                      ", 1); // ORB owns" << nl;
                  }
                else
                  {
                    *os << bt->name () << "_out ";
                  }
                break;
              } // end of inner switch
          }
        else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    // declare a variable
                    *os << bt->name () << "_ptr ";
                    *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", " << arg->local_name () <<
                      "); // ORB does not own" << nl;
                  }
                else
                  {
                    *os << bt->name () << "_ptr ";
                  }
                break;
              case AST_Argument::dir_INOUT:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    // declare a variable
                    *os << bt->name () << "_ptr ";
                    *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", " << arg->local_name () <<
                      ", 1); // ORB owns" << nl;
                  }
                else
                  {
                    *os << bt->name () << "_ptr &";
                  }
                break;
              case AST_Argument::dir_OUT:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    // declare a variable
                    *os << bt->name () << "_ptr ";
                    *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", " << arg->local_name () <<
                      ", 1); // ORB owns" << nl;
                  }
                else
                  {
                    *os << bt->name () << "_out ";
                  }
                break;
              }
          }
        else
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    // declare a variable
                    *os << bt->name () << " " << arg->local_name () << ";" <<
                      nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", &" << arg->local_name () <<
                      "); // ORB does not own" << nl;
                  }
                else
                  {
                    *os << bt->name ();
                  }
                break;
              case AST_Argument::dir_INOUT:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    // declare a variable
                    *os << bt->name () << " " << arg->local_name () << ";" <<
                      nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", &" << arg->local_name () <<
                      "); // ORB does not own" << nl;
                  }
                else
                  {
                    *os << bt->name () << " &";
                  }
                break;
              case AST_Argument::dir_OUT:
                if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
                  {
                    // declare a variable
                    *os << bt->name () << " " << arg->local_name () << ";" <<
                      nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", &" << arg->local_name () <<
                      ", 1); // ORB owns" << nl;
                  }
                else
                  {
                    *os << bt->name () << "_out";
                  }
                break;
              } // end of inner switch
          } // end of else
      } // end of case
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (arg->direction ())
          {
          case AST_Argument::dir_IN:
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                    // declare a variable
                *os << "char *" << arg->local_name () << ";" <<
                  nl;
                    // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                    // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", &" << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else
              {
                *os << "const char *";
              }
            break;
          case AST_Argument::dir_INOUT:
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                    // declare a variable
                *os << "char *" << arg->local_name () << ";" <<
                  nl;
                    // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                    // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", &" << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else
              {
                *os << "char *&";
              }
            break;
          case AST_Argument::dir_OUT:
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                // declare a variable
                *os << "char *" << arg->local_name () << ";" <<
                  nl;
                    // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                    // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", &" << arg->local_name () <<
                  ", 1); // ORB owns" << nl;
              }
            else
              {
                *os << bt->name () << "_out";
              }
            break;
          }
      }
      break;
    case AST_Decl::NT_array: // type is an array
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                // declare a variable
                *os << bt->name () << " " << arg->local_name ()
                    << ";" << nl;
                // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", " << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << "const " << bt->nested_type_name (bif);
              }
            else
              {
                *os << "const " << bt->name ();
              }
          }
          break;
        case AST_Argument::dir_INOUT:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                    // declare a variable
                *os << bt->name ();
                if (bt->size_type () == be_decl::VARIABLE)
                  {
                    *os << "_slice *";
                  }
                *os << " " << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                    // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", " << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                if (bt->size_type () == be_decl::VARIABLE)
                  {
                    *os << bt->nested_type_name (bif, "_slice") << " *";
                  }
                else
                  {
                    *os << bt->nested_type_name (bif);
                  }
              }
            else
              {
                *os << bt->name ();
                if (bt->size_type () == be_decl::VARIABLE)
                  {
                    *os << "_slice *";
                  }
              }
          }
          break;
        case AST_Argument::dir_OUT:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                    // declare a variable
                *os << bt->name () << "_slice *" << arg->local_name () << ";" <<
                  nl;
                    // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                    // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", " << arg->local_name () <<
                  ", 1); // ORB owns" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif, "_out") << " ";
              }
            else
              {
                *os << bt->name () << "_out";
              }
          }
          break;
        } // end of switch direction
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                // declare a variable
                *os << bt->name () << " " << arg->local_name () <<
                  ";" << nl;
                // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", &" << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << "const " << bt->nested_type_name (bif) << " &";
              }
            else
              {
                *os << "const " << bt->name () << " &";
              }
          }
          break;
        case AST_Argument::dir_INOUT:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                // declare a variable
                *os << bt->name () << " " << arg->local_name () <<
                  ";" << nl;
                // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", &" << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif) << " &";
              }
            else
              {
                *os << bt->name () << " &";
              }
          }
          break;
        case AST_Argument::dir_OUT:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                if (bt->size_type () == be_decl::VARIABLE)
                  {
                    // declare a variable
                    *os << bt->name () << " *" << arg->local_name () << ";" <<
                      nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", " << arg->local_name () <<
                      ", 1); // ORB owns" << nl;
                  }
                else
                  {
                    // declare a variable
                    *os << bt->name () << " " << arg->local_name () << ";" <<
                      nl;
                    // now define a NamedValue_ptr
                    *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                      ";" << nl;
                    // declare an Any
                    *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                      bt->tc_name () << ", &" << arg->local_name () <<
                      ", 1); // ORB owns" << nl;
                  }
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif, "_out") << " ";
              }
            else
              {
                *os << bt->name () << "_out";
                break;
              }
          }
          break;
        }
      break;
    case AST_Decl::NT_enum: // type is an enum
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                    // declare a variable
                *os << bt->name () << " " << arg->local_name () << ";" <<
                  nl;
                    // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                    // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", &" << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif);
              }
            else
              {
                *os << bt->name ();
              }
          }
          break;
        case AST_Argument::dir_INOUT:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                    // declare a variable
                *os << bt->name () << " " << arg->local_name () << ";" <<
                  nl;
                    // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                    // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", &" << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif) << " &";
              }
            else
              {
                *os << bt->name () << " &";
              }
          }
          break;
        case AST_Argument::dir_OUT:
          {
            if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS)
              {
                    // declare a variable
                *os << bt->name () << " " << arg->local_name () << ";" <<
                  nl;
                    // now define a NamedValue_ptr
                *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                  ";" << nl;
                    // declare an Any
                *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                  bt->tc_name () << ", &" << arg->local_name () <<
                  "); // ORB does not own" << nl;
              }
            else if (cg->state () == TAO_CodeGen::TAO_ARGUMENT_CH)
              {
                // to keep the MSVC++ compiler happy
                *os << bt->nested_type_name (bif, "_out") << " ";
              }
            else
              {
                *os << bt->name () << "_out";
              }
          }
          break;
        }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      } // end of switch
      break;
    }
  return 0;
}

// return type for typedef
be_state_typedef::be_state_typedef (void)
{
}

// generate code for structure member
int
be_state_typedef::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_typedef *tdef;  // typedef node
  be_decl *scope; // enclosing scope in which the typedef occurs

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      os = cg->client_header (); // set the stream to be the client header
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      os = cg->client_inline ();
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      os = cg->client_stubs ();
      break;
    default:
      return -1;
    } // end of outermost switch


  tdef = be_typedef::narrow_from_decl (d); // downcast to typedef node
  if (!tdef)
    return -1;

  // pass the typedef node, just in case it is needed
  cg->node (tdef);

  scope = be_decl::narrow_from_decl (ScopeAsDecl (tdef->defined_in ()));

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
    // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);


  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_interface_fwd: // type is an obj reference
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              os->indent (); // start from current indentation

              // typedef of an interface results in 3 typedefs as shown below
              *os << "typedef " << bt->nested_type_name (scope) << " " <<
                d->local_name () << ";" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_ptr") << " " <<
                d->local_name () << "_ptr;" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_var") << " " <<
                d->local_name () << "_var;" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_out") << " " <<
                d->local_name () << "_out;\n\n";
            }
            break;
          default:  // nothing to do for other cases
            break;
          } // end of switch state
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              be_predefined_type *pd = be_predefined_type::narrow_from_decl (bt);

              if (!pd)
                return -1;
              os->indent (); // start from current indentation
              *os << "typedef " << bt->nested_type_name (scope) << " " <<
                d->local_name () << ";";
              // if the predefined type is an ANY, we also define a typedef to _var
              if (pd->pt () == AST_PredefinedType::PT_any)
                {
                  *os << nl;
                  *os << "typedef " << bt->name () << "_var " << d->local_name
                    () << "_var;";
                }
              else if (pd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object
                  *os << "typedef " << bt->nested_type_name (scope, "_ptr") <<
                    " " << d->local_name () << "_ptr;" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_var") <<
                    " " << d->local_name () << "_var;" << nl;
                }
              *os << "typedef " << bt->nested_type_name (scope, "_out") << " "
                  << d->local_name () << "_out;\n\n";
            } // end of case
            break;
          default: // nothing to do for other cases
            break;
          } // end switch
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              os->indent (); // start from current indentation
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << "typedef " << bt->nested_type_name (scope) << " " <<
                    d->local_name () << ";" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_var") <<
                    " " << d->local_name () << "_var;" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_out") <<
                    " " << d->local_name () << "_out;\n\n";
                }
              else
                {
                  *os << "typedef CORBA::String " << d->local_name () << ";" <<
                    nl;
                  *os << "typedef CORBA::String_var " << d->local_name
                    () << "_var;" << nl;
                  *os << "typedef CORBA::String_out " << d->local_name
                    () << "_out;\n\n";
                }
            }
            break;
          default: // nothing to do for the rest of the cases
            break;
          } // end switch state
      }
      break;
    case AST_Decl::NT_enum: // type is an enum
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              os->indent (); // start from current indentation
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_state_typedef - enum gen failed\n"), -1);
                    }
                }
              *os << "typedef " << bt->nested_type_name (scope) << " " <<
                d->local_name () << ";" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_out") << " "
                  << d->local_name () << "_out;\n\n";
            }
            break;
          default:
            break;
          } // end of switch
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                           "be_state_typedef - array gen failed\n"), -1);
                    }
                }
              os->indent ();
              *os << "typedef " << bt->name () << " " << d->local_name () <<
                ";" << nl;
              *os << "typedef " << bt->name () << "_forany " << d->local_name
                () << "_forany;" << nl;
              // typedefs for the auxiliary methods. If we are nested inside
              // some scope, these methods become static to the enclosing scope
              if (d->is_nested ())
                *os << "static ";
              *os << d->name () << "_slice* " << d->local_name () <<
                "_alloc (void);" << nl;
              if (d->is_nested ())
                *os << "static ";
              *os << d->name () << "_slice* " << d->local_name () << "_dup ("
                  << d->name () << "_slice* " << ");" << nl;
              if (d->is_nested ())
                *os << "static ";
              *os << "void " << d->name () << "_free (" << d->name () <<
                "_slice *);\n\n";
            }
            break;
          default:
            break;
          } // end of switch
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - seq gen failed\n"),
                                        -1);
                    }
                }
            }
            break;
          case TAO_CodeGen::TAO_TYPEDEF_CI:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_inline () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - seq gen failed\n"),
                                        -1);
                    }
                }
            }
			break;
          case TAO_CodeGen::TAO_TYPEDEF_CS:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_stubs () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - seq gen failed\n"),
                                        -1);
                    }
                }
            }
			break;
          default:
            break;
          } // end of switch state
      }
      break;
    case AST_Decl::NT_struct:   // type is a struct
    case AST_Decl::NT_union:    // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - struct/union/seq gen failed\n"),
                                        -1);
                    }
                }
              os->indent (); // start from current indentation
              *os << "typedef " << bt->nested_type_name (scope) << " " <<
                d->local_name () << ";" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_var") << " "
                  << d->local_name () << "_var;" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_out") << " "
                  << d->local_name () << "_out;\n\n";
            }
            break;
          case TAO_CodeGen::TAO_TYPEDEF_CI:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_inline () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - struct/union/seq gen failed\n"),
                                        -1);
                    }
                }
            }
          case TAO_CodeGen::TAO_TYPEDEF_CS:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_stubs () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - struct/union/seq gen failed\n"),
                                        -1);
                    }
                }
            }
          default:
            break;
          } // end of switch state
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, tdef, temp);
      } // end of case
      break;
    } // end of switch

  return 0;
}

// return type for array
be_state_array::be_state_array (void)
{
}

// generate code for array type
int
be_state_array::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (nl);

  // get the appropriate stream
  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_ARRAY_DEFN_CH:
    case TAO_CodeGen::TAO_ARRAY_OTHER_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_ARRAY_DEFN_CI:
      os = cg->client_inline ();
      break;
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        *os << bt->name () << "_var ";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        *os << bt->name ();
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        *os << "CORBA::String_var ";
      }
      break;
    case AST_Decl::NT_array:
      // type is an array. This is possible only if we
      // are here thru the typedef node
      {
        *os << bt->name () << "_slice *";
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_enum:     // type is an enum
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct:   // type is a struct
    case AST_Decl::NT_union:    // type is a union
      {
        // based on what state we are in, we may have to generate the definition
        // of the type first
        if (cg->state () == TAO_CodeGen::TAO_ARRAY_DEFN_CH)
          if (bt->gen_client_header () == -1)  // generate the defn
            return -1;

        *os << bt->name ();
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      } // end of switch
      break;
    }
  return 0;
}

// return type for sequence
be_state_sequence::be_state_sequence (void)
{
}

// generate code for sequence type
int
be_state_sequence::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_sequence *seq;

  seq = be_sequence::narrow_from_decl (d);
  if (!seq)
    return -1;

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
      os = cg->client_header (); // get client header stream
      break;
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
      os = cg->client_stubs (); // get client stubs stream
      break;
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
      os = cg->client_inline (); // get client inline stream
      break;
    default:
      return -1;
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // for sequences, all we do is generate the type
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
            break;
          default:
            *os << bt->name () << "_var";
          }
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
            break;
          default:
            *os << bt->name ();
          }
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
            {
              // generate the base type sequence
              if (bt->gen_client_header () == -1)
                {
                  return -1;
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
            {
              // generate the base type sequence
              if (bt->gen_client_stubs () == -1)
                {
                  return -1;
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
            {
              // generate the base type sequence
              if (bt->gen_client_inline () == -1)
                {
                  return -1;
                }
            }
            break;
          default:
            {
              *os << bt->name ();
            }
          }
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG: Is this case valid ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      }
      break;
    } // end of switch
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<be_state_struct_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_disctypedefn_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_disctypedefn_ci, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_public_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_public_ci, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_union_private_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_operation, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_argument, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_typedef, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_array, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_sequence, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_state_attribute, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<TAO_OutStream_Factory, ACE_SYNCH_RECURSIVE_MUTEX>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
