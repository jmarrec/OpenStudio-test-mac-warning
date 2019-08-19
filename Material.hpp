#ifndef MODEL_MATERIAL_HPP
#define MODEL_MATERIAL_HPP

#include "IdfObject.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class Material_Impl;
} // detail

class Material : public openstudio::IdfObject {
 public:

  Material(std::string handle);

  virtual ~Material() {}

 protected:
  /// @cond
  typedef detail::Material_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::Material_Impl;

  explicit Material(std::shared_ptr<detail::Material_Impl> impl);
};

} // model
} // openstudio

#endif // MODEL_MATERIAL_HPP

