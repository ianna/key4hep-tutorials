#define MY_SOA_FIELDS \
  ((colA, float))     \
  ((colB, int))       \
  ((vec4, (std::array<float, 4>)))

#include "SoAToRNTupleBridge.hpp"

struct MySoA {
  float colA_;
  int colB_;
  std::array<float,4> vec4_;

  float colA() const { return colA_; }
  int colB() const { return colB_; }
  std::array<float,4> vec4() const { return vec4_; }
};

void writeRNTupleSingleObject(const MySoA& soa) {
  using namespace ROOT::Experimental;
  using namespace SoAToRNTuple;

  auto model = RNTupleModel::Create();

  SOA_DECLARE_FIELDS(model)

  auto writer = RNTupleWriter::Recreate(std::move(model), "ntuple", "data.root");

  SOA_ASSIGN_ALL_FROM_OBJ(soa)
  writer->Fill();
}

void writeRNTupleFromArray(const std::vector<MySoA>& vec) {
  using namespace ROOT::Experimental;
  using namespace SoAToRNTuple;

  auto model = RNTupleModel::Create();

  SOA_DECLARE_FIELDS(model)

  auto writer = RNTupleWriter::Recreate(std::move(model), "ntuple", "array_data.root");

  for (size_t i = 0; i < vec.size(); ++i) {
    SOA_ASSIGN_ALL_FROM_OBJ(vec[i])
    writer->Fill();
  }
}
