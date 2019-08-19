#ifndef UTILITIES_IDF_IDFOBJECT_HPP
#define UTILITIES_IDF_IDFOBJECT_HPP

#include <string>
#include <memory>

namespace openstudio{

// Forward declarations

namespace detail{
  class IdfObject_Impl;
}

class IdfObject {
  public:
    /** Constructor from type. Equivalent to IdfObject(IddFactory::instance.iddObject(type)),
     *  unless type == IddObjectType::UserCustom, in which case, the constructed object is equivalent
     *  to IdfObject(IddObjectType::Catchall). If fastName is true a UUID is assigned as the name,
     *  otherwise no name is assigned.*/
    explicit IdfObject(std::string handle);

    /** Copy constructor. Shares data with other, so changes made by either copy affects the data of
     *  both. */
    IdfObject(const IdfObject& other);

    virtual ~IdfObject() {}

    template<typename T>
    std::shared_ptr<T> getImpl() const {
      return std::dynamic_pointer_cast<T>(m_impl);
    }

    std::string handle() const;

  protected:

    typedef detail::IdfObject_Impl ImplType;
    friend struct IdfObjectImplLess;           // lets friend sort by impl
    friend class detail::IdfObject_Impl;       // for IdfObject_Impl::getObject<T>()

    /** Protected constructor from impl. */
    IdfObject(std::shared_ptr<detail::IdfObject_Impl> impl);


  private:
    // pointer to impl
    std::shared_ptr<detail::IdfObject_Impl> m_impl;
};

struct IdfObjectImplLess {
  /** Function object for sorting by impl. */
  bool operator()(const IdfObject& left, const IdfObject& right) const {
    return (left.getImpl<detail::IdfObject_Impl>() < right.getImpl<detail::IdfObject_Impl>());
  };
};

} // openstudio

#endif //UTILITIES_IDF_IDFOBJECT_HPP
