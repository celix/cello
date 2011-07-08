/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef Cellet_H
#define Cellet_H

#include <TProcessor.h>
#include "cellet_types.h"



class CelletIf {
 public:
  virtual ~CelletIf() {}
  virtual bool StartTask(const std::string& task_ad) = 0;
};

class CelletNull : virtual public CelletIf {
 public:
  virtual ~CelletNull() {}
  bool StartTask(const std::string& /* task_ad */) {
    bool _return = false;
    return _return;
  }
};

typedef struct _Cellet_StartTask_args__isset {
  _Cellet_StartTask_args__isset() : task_ad(false) {}
  bool task_ad;
} _Cellet_StartTask_args__isset;

class Cellet_StartTask_args {
 public:

  Cellet_StartTask_args() : task_ad("") {
  }

  virtual ~Cellet_StartTask_args() throw() {}

  std::string task_ad;

  _Cellet_StartTask_args__isset __isset;

  bool operator == (const Cellet_StartTask_args & rhs) const
  {
    if (!(task_ad == rhs.task_ad))
      return false;
    return true;
  }
  bool operator != (const Cellet_StartTask_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Cellet_StartTask_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Cellet_StartTask_pargs {
 public:


  virtual ~Cellet_StartTask_pargs() throw() {}

  const std::string* task_ad;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Cellet_StartTask_result__isset {
  _Cellet_StartTask_result__isset() : success(false) {}
  bool success;
} _Cellet_StartTask_result__isset;

class Cellet_StartTask_result {
 public:

  Cellet_StartTask_result() : success(0) {
  }

  virtual ~Cellet_StartTask_result() throw() {}

  bool success;

  _Cellet_StartTask_result__isset __isset;

  bool operator == (const Cellet_StartTask_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Cellet_StartTask_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Cellet_StartTask_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Cellet_StartTask_presult__isset {
  _Cellet_StartTask_presult__isset() : success(false) {}
  bool success;
} _Cellet_StartTask_presult__isset;

class Cellet_StartTask_presult {
 public:


  virtual ~Cellet_StartTask_presult() throw() {}

  bool* success;

  _Cellet_StartTask_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class CelletClient : virtual public CelletIf {
 public:
  CelletClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
    piprot_(prot),
    poprot_(prot) {
    iprot_ = prot.get();
    oprot_ = prot.get();
  }
  CelletClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
    piprot_(iprot),
    poprot_(oprot) {
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  bool StartTask(const std::string& task_ad);
  void send_StartTask(const std::string& task_ad);
  bool recv_StartTask();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class CelletProcessor : virtual public ::apache::thrift::TProcessor {
 protected:
  boost::shared_ptr<CelletIf> iface_;
  virtual bool process_fn(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, std::string& fname, int32_t seqid, void* callContext);
 private:
  std::map<std::string, void (CelletProcessor::*)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*)> processMap_;
  void process_StartTask(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  CelletProcessor(boost::shared_ptr<CelletIf> iface) :
    iface_(iface) {
    processMap_["StartTask"] = &CelletProcessor::process_StartTask;
  }

  virtual bool process(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot, void* callContext);
  virtual ~CelletProcessor() {}
};

class CelletMultiface : virtual public CelletIf {
 public:
  CelletMultiface(std::vector<boost::shared_ptr<CelletIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~CelletMultiface() {}
 protected:
  std::vector<boost::shared_ptr<CelletIf> > ifaces_;
  CelletMultiface() {}
  void add(boost::shared_ptr<CelletIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  bool StartTask(const std::string& task_ad) {
    uint32_t sz = ifaces_.size();
    for (uint32_t i = 0; i < sz; ++i) {
      if (i == sz - 1) {
        return ifaces_[i]->StartTask(task_ad);
      } else {
        ifaces_[i]->StartTask(task_ad);
      }
    }
  }

};



#endif
