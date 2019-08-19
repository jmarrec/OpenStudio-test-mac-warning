#ifndef UTILITIES_IDF_IDFOBJECT_IMPL_HPP
#define UTILITIES_IDF_IDFOBJECT_IMPL_HPP

#include <string>

namespace openstudio{

  // Forward declaration
  class IdfObject;

  // private namespace
namespace detail {

class IdfObject_Impl {
  public:
    /** Copy constructor, used for cloning. */
    IdfObject_Impl(const IdfObject_Impl& other);

    /** Constructor from type. Equivalent to IdfObject(IddFactory::instance.iddObject(type)). */
    explicit IdfObject_Impl(std::string handle);

    virtual ~IdfObject_Impl() {};

  protected:

    friend class openstudio::IdfObject;

  private:

    std::string m_handle;

    IdfObject_Impl(){}

};

} // detail

} // openstudio

#endif // UTILITIES_IDF_IDFOBJECT_IMPL_HPP
