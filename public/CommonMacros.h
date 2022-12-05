#pragma once

#include <boost/locale.hpp>
#include <Logger.h>


/** \file CommonMacros.h
 *  Common macros that will be used across projects
 * 
 *  \addtogroup CommonMacros
 *  @{
 */

//! Convenient to deleting vtk objects
#define DELETE_VTK_OBJECT(name)                                                                    \
  if (this->name) {                                                                                \
    this->name->Delete();                                                                          \
    this->name = nullptr;                                                                          \
  }

#define PfasToUnicode(v) boost::locale::conv::to_utf<wchar_t>(v, "utf-8")
#define PfasToMultibytes(v) boost::locale::conv::from_utf<wchar_t>(v, "utf-8")  
//! \name Variant to concrete types
//! @{
//! Macros to covert variant type to concrete types.
#define VARIANT_VALID(v) (v.vt != VT_EMPTY && v.vt != VT_NULL)
#define VARIANT_TO_STRING(v) PfasToMultibytes((std::wstring(static_cast<wchar_t *>(_bstr_t(v)))))
#define VARIANT_TO_BOOL(v) (static_cast<bool>(v))
#define VARIANT_TO_INT(v) (static_cast<int>(v))
#define VARIANT_TO_FLOAT(v) (static_cast<float>(v))
//! @}

//! @}