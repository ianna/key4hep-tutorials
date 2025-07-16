#ifndef SOA_TO_RNTUPLE_BRIDGE_HPP
#define SOA_TO_RNTUPLE_BRIDGE_HPP

#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RNTupleWriter.hxx>
#include <boost/preprocessor.hpp>
#include <array>
#include <memory>

namespace SoAToRNTuple {

using RModel = std::unique_ptr<ROOT::Experimental::RNTupleModel>;
using RFieldBase = std::shared_ptr<ROOT::Experimental::Detail::RFieldBase>;

// === USER DEFINES THIS MACRO ================================================
// Example:
// #define MY_SOA_FIELDS \
//   ((colA, float))     \
//   ((colB, int))       \
//   ((vec4, (std::array<float, 4>)))
// ============================================================================
#ifndef MY_SOA_FIELDS
#error "Please define MY_SOA_FIELDS macro before including SoAToRNTupleBridge.hpp"
#endif

// ----------------------------------------------------------------------------
// Create RNTuple fields from schema
// ----------------------------------------------------------------------------
#define SOA_MAKE_FIELD(r, model, elem)                                           \
  auto BOOST_PP_TUPLE_ELEM(2, 0, elem) =                                         \
      model->MakeField<BOOST_PP_TUPLE_ELEM(2, 1, elem)>(                         \
          #BOOST_PP_TUPLE_ELEM(2, 0, elem));

// Use this inside user code to declare fields
#define SOA_DECLARE_FIELDS(model)                                                \
  BOOST_PP_SEQ_FOR_EACH(SOA_MAKE_FIELD, model, MY_SOA_FIELDS)


// ----------------------------------------------------------------------------
// Assignment from SoA element (object with methods like colA(), colB(), etc.)
// ----------------------------------------------------------------------------
#define SOA_ASSIGN_FROM_OBJ(r, soa_obj, elem)                                    \
  *BOOST_PP_TUPLE_ELEM(2, 0, elem) =                                             \
      soa_obj.BOOST_PP_TUPLE_ELEM(2, 0, elem)();

#define SOA_ASSIGN_ALL_FROM_OBJ(soa_obj)                                         \
  BOOST_PP_SEQ_FOR_EACH(SOA_ASSIGN_FROM_OBJ, soa_obj, MY_SOA_FIELDS)


// ----------------------------------------------------------------------------
// Assignment from SoA with N elements, via ptr() accessor + index
// e.g. soa.colA_ptr()[i]
// ----------------------------------------------------------------------------
#define SOA_ASSIGN_FROM_INDEX(r, i, elem)                                        \
  *BOOST_PP_TUPLE_ELEM(2, 0, elem) =                                             \
      soa.BOOST_PP_TUPLE_ELEM(2, 0, elem)_ptr()[i];

#define SOA_ASSIGN_ALL_FROM_INDEXED(soa, i)                                      \
  BOOST_PP_SEQ_FOR_EACH(SOA_ASSIGN_FROM_INDEX, i, MY_SOA_FIELDS)

} // namespace SoAToRNTuple

#endif // SOA_TO_RNTUPLE_BRIDGE_HPP
