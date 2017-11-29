#ifndef ANALYSISTOOLS_UTILITIES_TREEWRITERDATA_H
#define ANALYSISTOOLS_UTILITIES_TREEWRITERDATA_H

#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <typeinfo>
#include <typeindex>
#include <stdexcept>

#include "AnalysisTools/Utilities/interface/TreeWriter.h"
#include "AnalysisBase/Analyzer/interface/prodIsoTrksTest.h"

namespace ucsbsusy {
class TreeWriter;

  class AbstractTreeVar {
  public:
    AbstractTreeVar(std::string prefix_, std::string name_) : prefix(prefix_), name(name_) {};
    virtual ~AbstractTreeVar() {};
    virtual void book(TreeWriter * tw) = 0;
    virtual void reset() = 0;
    std::string bookName() const {return prefix == "" ? name : (prefix + "_" + name); }
    const std::string prefix;
    const std::string name;
  };

  template<typename Type>
  class TreeVar : public AbstractTreeVar {
  public:
    TreeVar(const std::string prefix_, const std::string name_, const std::string type_, const Type defaultValue_)
    : AbstractTreeVar(prefix_,name_), type(type_) , defaultValue(defaultValue_), value(defaultValue)  { }
    void fill() { value = defaultValue;}
    void fill(const Type var) { value = var;}
    void set(const Type var) {fill(var);}
    void reset() { value = defaultValue;}
    void book(TreeWriter * tw) {tw->book(bookName().c_str(),value,type.c_str());}
  protected:
    const std::string type;
    const Type        defaultValue;
    Type              value;
  };

  template<typename Type>
  class TreeMultiVar : public AbstractTreeVar {
  public:
    TreeMultiVar(const std::string prefix_,const std::string name_,const Type defaultValue_)
      : AbstractTreeVar(prefix_,name_), defaultValue(defaultValue_), value(0) { }
    void fill() { value.push_back(defaultValue);}
    void fill(const Type var) { value.push_back(var);}
    void set(const std::vector<Type>& vars) {value = vars;}
    void reset() { unsigned int curSize = value.size(); value.resize(0); value.reserve(curSize);}
    void book(TreeWriter * tw) {tw->book(bookName().c_str(),value);}
  protected:
    const Type        defaultValue;
    std::vector<Type> value;
  };

  class TreeWriterData{
    using TypeMap = std::unordered_map<std::type_index, std::string>;
  public:
    TreeWriterData() : isBooked(false) {};
    ~TreeWriterData() { for(auto d : data) delete d; };

    template<typename Type>
    unsigned int add(const std::string prefix,const std::string name,const std::string type,const Type defaultValue){
      assert(!isBooked);
      if (var_pos.find(name) != var_pos.end()) throw std::invalid_argument("[TreeWriterData::add] Variable w/ the same name has already been added: "+name);
      data.push_back(new TreeVar<Type>(prefix,name,type,defaultValue));
      var_pos[name] = data.size() -1;
      return data.size() -1;
    }
    template<typename Type>
    unsigned int addMulti(const std::string prefix,const std::string name,const Type defaultValue){
      assert(!isBooked);
      if (var_pos.find(name) != var_pos.end()) throw std::invalid_argument("[TreeWriterData::addMulti] Variable w/ the same name has already been added: "+name);
      data.push_back(new TreeMultiVar<Type>(prefix,name,defaultValue));
      var_pos[name] = data.size() -1;
      return data.size() -1;
    }

    // simplified add
    template<typename Type>
    void add(const std::string name, const Type defaultValue){
      try {
        add<Type>("", name, type_names.at(std::type_index(typeid(Type))), defaultValue);
      } catch (const std::out_of_range& e){
        throw std::invalid_argument(std::string(typeid(Type).name()) + " is not a valid type for TTree!");
      }
    }
    template<typename Type>
    void addMulti(const std::string name, const Type defaultValue){
      addMulti("", name, defaultValue);
    }


    template<typename Type, typename FillType>
    void fill(unsigned int index, const FillType var){
      auto *tv=dynamic_cast<TreeVar<Type>*>(data.at(index));
      if (!tv) throw std::invalid_argument("[TreeWriterData::fill] Filling a value with wrong type to variable: "+data.at(index)->bookName()
          +"\nOr you might have forgotten to book some variables!");
      tv->fill(var);
    }
    template<typename Type>
    void fill(unsigned int index){
      auto *tv=dynamic_cast<TreeVar<Type>*>(data.at(index));
      if (!tv) throw std::invalid_argument("[TreeWriterData::fill] Filling a value with wrong type to variable: "+data.at(index)->bookName()
          +"\nOr you might have forgotten to book some variables!");
      tv->fill();
    }
    template<typename Type, typename FillType>
    void fillMulti(unsigned int index, const FillType var){
      auto *tv=dynamic_cast<TreeMultiVar<Type>*>(data.at(index));
      if (!tv) throw std::invalid_argument("[TreeWriterData::fillMulti] Filling a value with wrong type to variable: "+data.at(index)->bookName()
          +"\nOr you might have forgotten to book some variables!");
      tv->fill(var);
    }
    template<typename Type>
    void fillMulti(unsigned int index){
      auto *tv=dynamic_cast<TreeMultiVar<Type>*>(data.at(index));
      if (!tv) throw std::invalid_argument("[TreeWriterData::fillMulti] Filling a value with wrong type to variable: "+data.at(index)->bookName()
          +"\nOr you might have forgotten to book some variables!");
      tv->fill();
    }

    // string-based fill
    template<typename Type, typename FillType>
    void fill(const std::string name, const FillType var){
      try {
        fill<Type, FillType>(var_pos.at(name), var);
      } catch (const std::out_of_range& e){
        throw std::invalid_argument("[TreeWriterData::fill] Variable _" + name + "_ is not booked!");
      }
    }
    template<typename Type, typename FillType>
    void fillMulti(const std::string name, const FillType var){
      try {
        fillMulti<Type, FillType>(var_pos.at(name), var);
      } catch (const std::out_of_range& e){
        throw std::invalid_argument("[TreeWriterData::fillMulti] Variable _" + name + "_ is not booked!");
      }
    }

    template<typename Type>
    void set(const std::string name, const Type var){ fill<Type, Type>(name, var); }
    template<typename Type>
    void setMulti(const std::string name, const Type var){ fillMulti<Type, Type>(name, var); }


    void book(TreeWriter * tw){
      for(auto d : data) d->book(tw);
      isBooked = true;
    }
    void reset(){ for(auto d : data) d->reset();}
  protected:
    bool isBooked;
    std::vector<AbstractTreeVar*> data;
    std::unordered_map<std::string, unsigned int> var_pos;
    static TypeMap type_names;
  };
}

#endif
