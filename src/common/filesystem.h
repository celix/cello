#ifndef SRC_TOOLS_FILESYSTEM_H
#define SRC_TOOLS_FILESYSTEM_H

#include <string>

using std::string;

class FileSystem {
public:
    virtual ~FileSystem() {}
    
    virtual int Connect(const string& ip, int port) = 0;
    
    virtual void Disconnect() = 0;

    virtual int CopyFromLocalToDfs(const string& src_path, const string& dest_path) = 0;

    virtual int CopyFromDfsToLocal(const string& src_path, const string& dest_path) = 0;
    
};

#endif
