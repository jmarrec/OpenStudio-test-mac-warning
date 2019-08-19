#include "Material.hpp"
#include "Material_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

} // detail

Material::Material(std::string handle) : IdfObject(handle) {};

/// @cond
Material::Material(std::shared_ptr<detail::Material_Impl> impl)
  : IdfObject(std::move(impl)) {};
/// @endcond

} // model
} // openstudio
