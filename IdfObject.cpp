#include "IdfObject.hpp"
#include "IdfObject_Impl.hpp"

namespace openstudio {

namespace detail {

  IdfObject_Impl::IdfObject_Impl(const IdfObject_Impl& other)
    : m_handle(other.m_handle) {};

  /** Constructor from type. Equivalent to IdfObject(IddFactory::instance.iddObject(type)). */
  IdfObject_Impl::IdfObject_Impl(std::string handle)
    : m_handle(handle) {};

} // detail

IdfObject::IdfObject(std::string handle) {
  m_impl = std::shared_ptr<detail::IdfObject_Impl>(new detail::IdfObject_Impl(handle));
};

/** Copy constructor. Shares data with other, so changes made by either copy affects the data of
 *  both. */
IdfObject::IdfObject(const IdfObject& other)
  : m_impl(other.m_impl) {};

std::string IdfObject::handle() const { return getImpl<detail::IdfObject_Impl>()->m_handle; };

// Protected
IdfObject::IdfObject(std::shared_ptr<detail::IdfObject_Impl> impl)
  : m_impl(std::move(impl)) {};

} // openstudio
