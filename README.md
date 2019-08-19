# Incomplete type as key in std::set is making Mac throw

Two branches: **master** (fails on mac, works fine on GCC), and **fixed** where I include `Material.hpp` in `ForwardTranslator.hpp` where `std::set<Material, IdfObjectImplLess>` is defined, and works on mac too.


## Original problems

Jenkins was throwing on mac only after enabling `-Werror`, cf https://ci.commercialbuildings.dev/blue/organizations/jenkins/openstudio-incremental-develop3/detail/PR-3587/30/pipeline/8/#step-89-log-717:

```
In file included from /Users/jenkins/git/OpenStudioIncremental/develop3/src/gbxml/ForwardTranslator.cpp:30:
In file included from /Users/jenkins/git/OpenStudioIncremental/develop3/src/gbxml/ForwardTranslator.hpp:37:
In file included from /Users/jenkins/git/OpenStudioIncremental/develop3/src/gbxml/../model/../utilities/core/Logger.hpp:37:
In file included from /Users/jenkins/git/OpenStudioIncremental/develop3/src/gbxml/../utilities/core/Compare.hpp:41:
In file included from /Users/jenkins/.conan/data/boost_cycle_group_a/1.69.0/bincrafters/stable/package/641514453a9f65f8e0c7dfc14fa6b881970a8add/algorithm/include/boost/algorithm/string.hpp:23:
In file included from /Users/jenkins/.conan/data/boost_cycle_group_a/1.69.0/bincrafters/stable/package/641514453a9f65f8e0c7dfc14fa6b881970a8add/algorithm/include/boost/algorithm/string/split.hpp:16:
In file included from /Users/jenkins/.conan/data/boost_cycle_group_a/1.69.0/bincrafters/stable/package/641514453a9f65f8e0c7dfc14fa6b881970a8add/algorithm/include/boost/algorithm/string/iter_find.hpp:27:
In file included from /Users/jenkins/.conan/data/boost_cycle_group_a/1.69.0/bincrafters/stable/package/641514453a9f65f8e0c7dfc14fa6b881970a8add/algorithm/include/boost/algorithm/string/find_iterator.hpp:24:
In file included from /Users/jenkins/.conan/data/boost_cycle_group_a/1.69.0/bincrafters/stable/package/641514453a9f65f8e0c7dfc14fa6b881970a8add/algorithm/include/boost/algorithm/string/detail/find_iterator.hpp:18:
In file included from /Users/jenkins/.conan/data/boost_function/1.69.0/bincrafters/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/function/include/boost/function.hpp:30:
In file included from /Users/jenkins/.conan/data/boost_function/1.69.0/bincrafters/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/function/include/boost/function/detail/prologue.hpp:17:
In file included from /Users/jenkins/.conan/data/boost_function/1.69.0/bincrafters/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/function/include/boost/function/function_base.hpp:21:
In file included from /Users/jenkins/.conan/data/boost_type_index/1.69.0/bincrafters/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/type_index/include/boost/type_index.hpp:29:
In file included from /Users/jenkins/.conan/data/boost_type_index/1.69.0/bincrafters/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/type_index/include/boost/type_index/stl_type_index.hpp:47:
In file included from /Users/jenkins/.conan/data/boost_container_hash/1.69.0/bincrafters/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/container_hash/include/boost/container_hash/hash.hpp:760:
In file included from /Users/jenkins/.conan/data/boost_container_hash/1.69.0/bincrafters/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/container_hash/include/boost/container_hash/extensions.hpp:22:
In file included from /Users/jenkins/.conan/data/boost_detail/1.69.0/bincrafters/stable/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/detail/include/boost/detail/container_fwd.hpp:95:
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/set:426:26: error: the specified comparator type does not provide a const call operator [-Werror,-Wuser-defined-warnings]
    static_assert(sizeof(__diagnose_non_const_comparator<_Key, _Compare>()), "");
                         ^
/Users/jenkins/git/OpenStudioIncremental/develop3/src/gbxml/ForwardTranslator.hpp:110:74: note: in instantiation of template class 'std::__1::set<openstudio::model::Material, openstudio::IdfObjectImplLess, std::__1::allocator<openstudio::model::Material> >' requested here
    std::set<openstudio::model::Material, openstudio::IdfObjectImplLess> m_materials;

/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/__tree:967:5: note: from 'diagnose_if' attribute on '__diagnose_non_const_comparator<openstudio::model::Material, openstudio::IdfObjectImplLess>':
    _LIBCPP_DIAGNOSE_WARNING(!std::__invokable<_Compare const&, _Tp const&, _Tp const&>::value,
    ^                        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/__config:1251:21: note: expanded from macro '_LIBCPP_DIAGNOSE_WARNING'
     __attribute__((diagnose_if(__VA_ARGS__, "warning")))
                    ^           ~~~~~~~~~~~
1 error generated.
make[2]: *** [src/gbxml/CMakeFiles/openstudio_gbxml.dir/ForwardTranslator.cpp.o] Error 1
make[1]: *** [src/gbxml/CMakeFiles/openstudio_gbxml.dir/all] Error 2
make[1]: *** Waiting for unfinished jobs....
```

----


I couldn't figure out the error, since the comparator `IdfObjectImplLess` did seem to be `const`, as well as any functions called in the comparator (`IdfObject::getImpl`).

[gbxml/ForwardTranslator.hpp](https://github.com/NREL/OpenStudio/blob/develop3/src/gbxml/ForwardTranslator.hpp#L110):

```
std::set<openstudio::model::Material, openstudio::IdfObjectImplLess> m_materials;
```


[IdfObject.hpp](https://github.com/NREL/OpenStudio/blob/develop3/src/utilities/idf/IdfObject.hpp#L433:L436)

```
/** Function object for sorting objects by impl. \relates IdfObject */
struct UTILITIES_API IdfObjectImplLess {
  bool operator()(const IdfObject& left, const IdfObject& right) const;
};
```

[IdfObject.cpp](https://github.com/NREL/OpenStudio/blob/develop3/src/utilities/idf/IdfObject.cpp#L2387:L2390):

```
/** Function object for sorting by impl. */
bool IdfObjectImplLess::operator()(const IdfObject& left, const IdfObject& right) const {
  return (left.getImpl<detail::IdfObject_Impl>() < right.getImpl<detail::IdfObject_Impl>());
}
```


IdfObject has a private impl: `std::shared_ptr<detail::IdfObject_Impl> m_impl;`

[IdfObject.hpp: IdfObject::getImpl](https://github.com/NREL/OpenStudio/blob/develop3/src/utilities/idf/IdfObject.hpp#L378:L380)

```
  template<typename T>
    std::shared_ptr<T> getImpl() const
  {  return std::dynamic_pointer_cast<T>(m_impl); }
```

## Solution

At the point of declaration of `std::set<Material, IdfObjectImplLess>`, in `gbxml/ForwardTranslator.hpp`, `Material` was just forward-declared so it's an incomplete type, and in particular the compiler can't know that `Material` derives from `IdfObject` and will not correctly resolve the `operator()` and throw this `error: the specified comparator type does not provide a const call operator [-Werror,-Wuser-defined-warnings]`.
