#include "AnalysisTools/Utilities/interface/TreeWriterData.h"

using namespace ucsbsusy;

TreeWriterData::TypeMap TreeWriterData::type_names = []{
    TypeMap tmap;
    tmap[std::type_index(typeid(char))]                = "B";
    tmap[std::type_index(typeid(unsigned char))]       = "b";
    tmap[std::type_index(typeid(short))]               = "S";
    tmap[std::type_index(typeid(unsigned short))]      = "s";
    tmap[std::type_index(typeid(int))]                 = "I";
    tmap[std::type_index(typeid(unsigned int))]        = "i";
    tmap[std::type_index(typeid(float))]               = "F";
    tmap[std::type_index(typeid(double))]              = "D";
    tmap[std::type_index(typeid(long long))]           = "L";
    tmap[std::type_index(typeid(unsigned long long))]  = "l";
    tmap[std::type_index(typeid(bool))]                = "O";
    return tmap;
}();
