/**
 * Non-metric Space Library
 *
 * Authors: Bilegsaikhan Naidan (https://github.com/bileg), Leonid Boytsov (http://boytsov.info).
 * With contributions from Lawrence Cayton (http://lcayton.com/) and others.
 *
 * For the complete list of contributors and further details see:
 * https://github.com/searchivarius/NonMetricSpaceLib 
 * 
 * Copyright (c) 2018
 *
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 */

#ifndef _SPACE_SIFT_VECTOR_H_
#define _SPACE_SIFT_VECTOR_H_

#include <string>
#include <map>
#include <stdexcept>
#include <sstream>
#include <memory>

#include <string.h>
#include "global.h"
#include "object.h"
#include "utils.h"
#include "space.h"
#include "distcomp.h"

namespace similarity {

using std::string;
using std::unique_ptr;

class SiftVectorSpace : public Space<DistTypeSift> {
 public:
  explicit SiftVectorSpace() {}
  virtual ~SiftVectorSpace() {}

  /** Standard functions to read/write/create objects */ 
  virtual unique_ptr<Object> CreateObjFromStr(IdType id, LabelType label, const string& s,
                                                DataFileInputState* pInpState) const;
    // Create a string representation of an object.
    virtual string CreateStrFromObj(const Object* pObj, const string& externId /* ignored */) const;
    // Open a file for reading, fetch a header (if there is any) and memorize an input state
    virtual unique_ptr<DataFileInputState> OpenReadFileHeader(const string& inputFile) const;
    // Open a file for writing, write a header (if there is any) and memorize an output state
    virtual unique_ptr<DataFileOutputState> OpenWriteFileHeader(const ObjectVector& dataset,
                                                                const string& outputFile) const;
    /*
     * Read a string representation of the next object in a file as well
     * as its label. Return false, on EOF.
     */
    virtual bool ReadNextObjStr(DataFileInputState &, string& strObj, LabelType& label, string& externId) const;
  /** End of standard functions to read/write/create objects */ 

  virtual Object* CreateObjFromUint8Vect(IdType id, LabelType label, const std::vector<uint8_t>& InpVect) const;
  virtual size_t GetElemQty(const Object* object) const { return SIFT_DIM; }

  static void ReadUint8Vec(std::string line, LabelType& label, std::vector<uint8_t>& v);

protected:
  DISABLE_COPY_AND_ASSIGN(VectorSpace);

  virtual DistTypeSift HiddenDistance(const Object* obj1, const Object* obj2) const override {
    const uint8_t* pVect1 = reinterpret_cast<const uint8_t*>(obj1->data());
    const uint8_t* pVect2 = reinterpret_cast<const uint8_t*>(obj2->data());

    return l2SqrSIFTPrecompAVX(pVect1, pVect2);
  }
};

}  // namespace similarity

#endif
