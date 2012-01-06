#ifndef SRC_TOOLS_HDFS_FILESYSTEM_H
#define SRC_TOOLS_HDFS_FILESYSTEM_H

#include "common/filesystem.h"
#include "common/register.h"

#include <hdfs/hdfs.h>

/* HDFS file system class */

class HdfsFileSytem : public FileSystem {
public:
    HdfsFileSytem(): m_handler(0) {}

    int Connect(const string& ip, int port);
   
    void Disconnect();

    int CopyFromLocalToDfs(const string& src_path, const string& dest_path);

    int CopyFromDfsToLocal(const string& src_path, const string& dest_path);

private:
    hdfsFS m_handler;
};

REGISTER_CLASS(HdfsFileSytem);

#endif
