#ifndef ANALYSISTOOLS_UTILITIES_TREEWRITERDATA_H
#define ANALYSISTOOLS_UTILITIES_TREEWRITERDATA_H

#include <string>
#include <vector>
#include <assert.h>


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
  public:
    TreeWriterData() : isBooked(false) {};
    ~TreeWriterData() { for(auto d : data) delete d; };

    template<typename Type>
    unsigned int add(const std::string prefix,const std::string name,const std::string type,const Type defaultValue){
      assert(!isBooked);
      data.push_back(new TreeVar<Type>(prefix,name,type,defaultValue));
      return data.size() -1;
    }
    template<typename Type>
    unsigned int addMulti(const std::string prefix,const std::string name,const Type defaultValue){
      assert(!isBooked);
      data.push_back(new TreeMultiVar<Type>(prefix,name,defaultValue));
      return data.size() -1;
    }

    template<typename Type, typename FillType>
    void fill(unsigned int index, const FillType var){assert(index < data.size()); ((TreeVar<Type>*)(data[index]))->fill(var);}
    template<typename Type>
    void fill(unsigned int index){ assert(index < data.size()); ((TreeVar<Type>*)(data[index]))->fill();}
    template<typename Type, typename FillType>
    void fillMulti(unsigned int index, const FillType var){assert(index < data.size()); ((TreeMultiVar<Type>*)(data[index]))->fill(var);}
    template<typename Type>
    void fillMulti(unsigned int index){assert(index < data.size()); ((TreeMultiVar<Type>*)(data[index]))->fill();}

    void book(TreeWriter * tw){
      for(auto d : data) d->book(tw);
      isBooked = true;
    }
    void reset(){ for(auto d : data) d->reset();}
  protected:
    bool isBooked;
    std::vector<AbstractTreeVar*> data;
  };
}

#endif
