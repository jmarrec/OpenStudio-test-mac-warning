#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <memory>


namespace openstudio{

  // Forward declaration
  class IdfObject;

  // private namespace
namespace detail {

class IdfObject_Impl {
  public:
    /** Copy constructor, used for cloning. */
    IdfObject_Impl(const IdfObject_Impl& other)
      : m_handle(other.m_handle) {};

    /** Constructor from type. Equivalent to IdfObject(IddFactory::instance.iddObject(type)). */
    explicit IdfObject_Impl(std::string handle) {
      m_handle = handle;
    };

    virtual ~IdfObject_Impl() {};

  protected:

    friend class openstudio::IdfObject;

  private:

    std::string m_handle;

    IdfObject_Impl(){}

};

class Material_Impl : public IdfObject_Impl { };

} // detail

class IdfObjectImplLess;

class IdfObject {
  public:
    /** Constructor from type. Equivalent to IdfObject(IddFactory::instance.iddObject(type)),
     *  unless type == IddObjectType::UserCustom, in which case, the constructed object is equivalent
     *  to IdfObject(IddObjectType::Catchall). If fastName is true a UUID is assigned as the name,
     *  otherwise no name is assigned.*/
    explicit IdfObject(std::string handle) {
      m_impl = std::shared_ptr<detail::IdfObject_Impl>(new detail::IdfObject_Impl(handle));
    };

    /** Copy constructor. Shares data with other, so changes made by either copy affects the data of
     *  both. */
    IdfObject(const IdfObject& other)
      : m_impl(other.m_impl)
    {
    };

    virtual ~IdfObject() {}

    template<typename T>
    std::shared_ptr<T> getImpl() const {
      return std::dynamic_pointer_cast<T>(m_impl);
    }

    std::string handle() const { return getImpl<detail::IdfObject_Impl>()->m_handle; };

  protected:

    typedef detail::IdfObject_Impl ImplType;
    friend struct IdfObjectImplLess;           // lets friend sort by impl
    friend class detail::IdfObject_Impl;       // for IdfObject_Impl::getObject<T>()

    /** Protected constructor from impl. */
    IdfObject(std::shared_ptr<detail::IdfObject_Impl> impl) : m_impl(std::move(impl)) {};


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


class Material : public IdfObject {
 public:

  Material(std::string handle) : IdfObject(handle) {};
  virtual ~Material() {}

 protected:
  /// @cond
  typedef detail::Material_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::Material_Impl;

  explicit Material(std::shared_ptr<detail::Material_Impl> impl) : IdfObject(std::move(impl)) {};
};

} // openstudio


using namespace openstudio;

int main()
{

  std::set<Material, IdfObjectImplLess> s;
  Material A("A");
  Material B("B");
  Material B_bis(B); // Copy constructor
  // Material B_bis(B_impl);
  s.insert(A);
  s.insert(B);
  s.insert(B_bis);

  std::cout << "B = " << B.handle() << ", B_bis = " << B_bis.handle() << std::endl;

  std::cout << std::boolalpha << "Set size is 2: " << (s.size() == 2) << std::endl;

}

