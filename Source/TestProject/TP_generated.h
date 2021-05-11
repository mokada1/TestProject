// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TP_H_
#define FLATBUFFERS_GENERATED_TP_H_

#include "flatbuffers/flatbuffers.h"

struct Vec3;

struct REQ_MOVE;
struct REQ_MOVEBuilder;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3() {
    memset(static_cast<void *>(this), 0, sizeof(Vec3));
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vec3, 12);

struct REQ_MOVE FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef REQ_MOVEBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CHARACTERID = 4,
    VT_POS = 6
  };
  int32_t CharacterId() const {
    return GetField<int32_t>(VT_CHARACTERID, 0);
  }
  const Vec3 *Pos() const {
    return GetStruct<const Vec3 *>(VT_POS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_CHARACTERID) &&
           VerifyField<Vec3>(verifier, VT_POS) &&
           verifier.EndTable();
  }
};

struct REQ_MOVEBuilder {
  typedef REQ_MOVE Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_CharacterId(int32_t CharacterId) {
    fbb_.AddElement<int32_t>(REQ_MOVE::VT_CHARACTERID, CharacterId, 0);
  }
  void add_Pos(const Vec3 *Pos) {
    fbb_.AddStruct(REQ_MOVE::VT_POS, Pos);
  }
  explicit REQ_MOVEBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  REQ_MOVEBuilder &operator=(const REQ_MOVEBuilder &);
  flatbuffers::Offset<REQ_MOVE> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<REQ_MOVE>(end);
    return o;
  }
};

inline flatbuffers::Offset<REQ_MOVE> CreateREQ_MOVE(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t CharacterId = 0,
    const Vec3 *Pos = 0) {
  REQ_MOVEBuilder builder_(_fbb);
  builder_.add_Pos(Pos);
  builder_.add_CharacterId(CharacterId);
  return builder_.Finish();
}

#endif  // FLATBUFFERS_GENERATED_TP_H_
