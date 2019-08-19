#ifndef GBXML_FORWARDTRANSLATOR_HPP
#define GBXML_FORWARDTRANSLATOR_HPP

#include "IdfObject.hpp"

#include <set>

namespace openstudio {
  namespace model {
    class Material;
  }

namespace gbxml {

class ForwardTranslator {
 public:
  ForwardTranslator();

  virtual ~ForwardTranslator();

  std::set<openstudio::model::Material, openstudio::IdfObjectImplLess> m_materials;
};

} // gbxml
} // openstudio

#endif // GBXML_FORWARDTRANSLATOR_HPP
