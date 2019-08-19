#include <iostream>
#include <sstream>

#include "ForwardTranslator.hpp"
#include "Material.hpp"
#include "Material_Impl.hpp"

using namespace openstudio;

int main()
{

  model::Material A("A");
  model::Material B("B");
  model::Material B_bis(B); // Copy constructor
  // Material B_bis(B_impl);

  gbxml::ForwardTranslator ft;
  std::set<model::Material, IdfObjectImplLess> &s = ft.m_materials;
  s.insert(A);
  s.insert(B);
  s.insert(B_bis);

  std::cout << "B = " << B.handle() << ", B_bis = " << B_bis.handle() << std::endl;

  std::cout << std::boolalpha << "Set size is 2: " << (s.size() == 2) << std::endl;

}

