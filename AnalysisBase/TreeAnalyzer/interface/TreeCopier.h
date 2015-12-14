
#ifndef ANALYSISBASE_TREEANALYZER_TREECOPIER_H
#define ANALYSISBASE_TREEANALYZER_TREECOPIER_H

#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TFile.h>

#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/TreeWriterData.h"
#include "AnalysisTools/Utilities/interface/TreeWriter.h"


namespace ucsbsusy {

class CopierEventAnalyzer : public BaseEventAnalyzer {
public:
  CopierEventAnalyzer() {};
  virtual ~CopierEventAnalyzer() {};
  virtual void analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency = 10000, int numEvents = -1);
};


class CopierFillYourselfEventAnalyzer : public BaseEventAnalyzer {
public:
  CopierFillYourselfEventAnalyzer() {};
  virtual ~CopierFillYourselfEventAnalyzer() {};
  virtual void analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency = 10000, int numEvents = -1);
};


//--------------------------------------------------------------------------------------------------
// Copy trees directly, with all or a subset of branches
//--------------------------------------------------------------------------------------------------

  class TreeCopier : public BaseTreeAnalyzer {
  public:

    TreeCopier(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet * pars);
    virtual ~TreeCopier();

    virtual BaseEventAnalyzer * setupEventAnalyzer() override {return new CopierEventAnalyzer();}


    virtual void setupTree() = 0; //Used by the child classes
    virtual void book() {}; //if you want to book your own variables
    virtual bool fillEvent() {return true;} //whether or not to fill the event, also fill your custom vars

    //helper functions for tree work
    void bookFillingTree() {data.book(treeWriter_);}
    void resetFillingData() {data.reset();}
    void fillFillingTree() {outFile_->cd(); treeWriter_->fill(); }
    TreeWriterData * fillingData() {return &data;}

  private:
    void runEvent() {}; //Never used
  protected:
    const TString outFileName_;

    TFile*          outFile_;
    TreeWriter*     treeWriter_;
    TreeWriterData  data;
  };

  class TreeCopierAllBranches : public TreeCopier {
  public:
    TreeCopierAllBranches(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree,cfgSet::ConfigSet * pars) :
      TreeCopier(fileName,treeName,outFileName,randomSeed,isMCTree,pars) {}

    virtual void setupTree() {
      reader.getTree()->SetBranchStatus("*",1);
      outFile_ = new TFile(outFileName_,"RECREATE");
      outFile_->cd();
      treeWriter_ = new TreeWriter(reader.getTree()->CloneTree(0));
    }
  };

  class TreeCopierLoadedBranches : public TreeCopier {
  public:
    TreeCopierLoadedBranches(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet * pars) :
      TreeCopier(fileName,treeName,outFileName,randomSeed,isMCTree,pars) {}

    virtual void setupTree() {
      outFile_ = new TFile(outFileName_,"RECREATE");
      outFile_->cd();
      treeWriter_ = new TreeWriter(reader.getTree()->CloneTree(0));
    }
  };

  class TreeCopierManualBranches : public TreeCopier {
  public:
    TreeCopierManualBranches(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet * pars) :
      TreeCopier(fileName,treeName,outFileName,randomSeed,isMCTree,pars) {}
  public:
    virtual void setupTree() {
      outFile_ = new TFile(outFileName_,"RECREATE");
      outFile_->cd();
      treeWriter_ = new TreeWriter(new TTree(reader.getTree()->GetName(),reader.getTree()->GetTitle()),reader.getTree()->GetName() );
    }
  };

  //--------------------------------------------------------------------------------------------------
  // Flatten a Tree filled with vectors to a flat one
  //--------------------------------------------------------------------------------------------------
  class AbstractLinkedTreeVar {
  public:
    AbstractLinkedTreeVar(std::string name_,std::string type_) :name(name_), type(type_) {};
    virtual ~AbstractLinkedTreeVar() {};
    virtual void book(TreeWriter * tw) = 0;
    virtual void fill(size i) = 0;
    virtual size  varSize() const = 0;
    const std::string name;
    const std::string type;

  };

  template<typename Type>
  class TreeLinkedVar : public AbstractLinkedTreeVar {
  public:
    TreeLinkedVar(const std::string name_,const std::string type_, const Type* inData_)
      : AbstractLinkedTreeVar(name_,type_), inData(inData_),value(0) { }
    void book(TreeWriter * tw) {tw->book(name.c_str(),value, type.c_str());}
    void fill(size i) { value = (*inData);}
    size varSize() const {return 0;}
  protected:
    const Type* inData;
    Type        value;
  };
  template<typename Type>
  class TreeLinkedMultiVar : public AbstractLinkedTreeVar {
  public:
    TreeLinkedMultiVar(const std::string name_, const std::string type_, const std::vector<Type>* inData_)
      : AbstractLinkedTreeVar(name_,type_),inData(inData_),value(0) { }
    void book(TreeWriter * tw) {tw->book(name.c_str(),value, type.c_str());}
    void fill(size i) { value = inData->at(i);}
    size  varSize() const { return inData->size();}
  protected:
    const std::vector<Type>* inData;
    Type        value;
  };

  class TreeLinkedWriterData{
  public:
    TreeLinkedWriterData() : isBooked(false), countingVar(0) {};
    ~TreeLinkedWriterData() { for(auto d : newData) delete d; for(auto d : linkedData) delete d; for(auto d : linkedMultiData) delete d; };

    template<typename Type>
    size addNew(const std::string prefix,const std::string name,const std::string type,const Type defaultValue){
      assert(!isBooked);
      newData.push_back(new TreeVar<Type>(prefix,name,type,defaultValue));
      return newData.size() -1;
    }
    template<typename Type>
    void addLinked(const std::string name, std::string type, const Type * inData){
      assert(!isBooked);
      linkedData.push_back(new TreeLinkedVar<Type>(name,type,inData));
    }
    template<typename Type>
    void addLinkedMulti(const std::string name, std::string type, const std::vector<Type>* inData){
      assert(!isBooked);
      linkedMultiData.push_back(new TreeLinkedMultiVar<Type>(name,type,inData));
      countingVar = linkedMultiData.back();
    }

    template<typename Type, typename FillType>
    void fill(size index, const FillType var){assert(index < newData.size()); ((TreeVar<Type>*)(newData[index]))->fill(var);}
    template<typename Type>
    void fill(size index){ assert(index < newData.size()); ((TreeVar<Type>*)(newData[index]))->fill();}

    void fillLinked(){for(auto d : linkedData) d->fill(0);  }
    void fillLinkedMulti(size i){for(auto d : linkedMultiData) d->fill(i);  }
    size getVecSize() const {return countingVar == 0 ? 0 : countingVar->varSize(); }

    void book(TreeWriter * tw){
      for(auto d : newData) d->book(tw);
      for(auto d : linkedData) d->book(tw);
      for(auto d : linkedMultiData) d->book(tw);
      isBooked = true;
    }
    void reset(){ for(auto d : newData) d->reset();}
  protected:
    bool isBooked;
    AbstractLinkedTreeVar * countingVar;
    std::vector<AbstractTreeVar*> newData;
    std::vector<AbstractLinkedTreeVar*> linkedData;
    std::vector<AbstractLinkedTreeVar*> linkedMultiData;
  };


  class FlattenCopierEventAnalyzer : public BaseEventAnalyzer {
  public:
    FlattenCopierEventAnalyzer() {};
    virtual ~FlattenCopierEventAnalyzer() {};
    virtual void analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency = 10000, int numEvents = -1);
  };


  class TreeFlattenCopier : public BaseTreeAnalyzer {
  public:
    TreeFlattenCopier(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet * pars);
    virtual ~TreeFlattenCopier();

    virtual BaseEventAnalyzer * setupEventAnalyzer() {return new FlattenCopierEventAnalyzer();}

    template<typename Type>
    void addLinked(const Type * inData, const std::string type){
      std::string branchName = reader.getBranchName(inData);
      if(branchName == "") throw std::invalid_argument("TreeFlattenCopier::addLinked not a valid variable!");
      return data.addLinked(branchName,type,inData);
    }

    template<typename Type>
    void addLinkedMulti(const std::vector<Type>* inData, const std::string type){
      std::string branchName = reader.getBranchName(inData);
      if(branchName == "") throw std::invalid_argument("TreeFlattenCopier::addLinked not a valid variable!");
      return data.addLinkedMulti(branchName,type,inData);
    }

    template<typename Type>
    void addLinkedMulti(const std::vector<Type>* inData, const std::string name,const std::string type){
      return data.addLinkedMulti(name,type,inData);
    }

    virtual void book() {}; //if you want to book your own variables
    virtual bool fillEvent() {return true;} //whether or not to fill the event, also fill your custom vars

    void bookFillingTree() {data.book(treeWriter_);}
    void resetFillingData() {data.reset();}
    void fillFillingTree();

  private:
    void runEvent() {}; //Never used
  protected:
    const TString outFileName_;

    TFile*          outFile_;
    TreeWriter*     treeWriter_;
    TreeLinkedWriterData  data;
  };




}


#endif
