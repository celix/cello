#include "common/hdfs_filesystem.h"

int HdfsFileSytem::Connect(const string& ip, int port) {
    m_handler = hdfsConnect(ip.c_str(), port);
    return (m_handler ? 0 : -1);
}

void HdfsFileSytem::Disconnect() {
    hdfsDisconnect(m_handler);
}

int HdfsFileSytem::CopyFromLocalToDfs(const string& src_path,
                                      const string& dest_path) {
    // connect to local filesystem
    hdfsFS local_handler = hdfsConnect(NULL, 0);
    int ret = hdfsCopy(local_handler, src_path.c_str(), m_handler, dest_path.c_str());
    hdfsDisconnect(local_handler);
    return ret;
}

int HdfsFileSytem::CopyFromDfsToLocal(const string& src_path,
                                      const string& dest_path) {
    // connect to local filesystem
    hdfsFS local_handler = hdfsConnect(NULL, 0);
    int ret = hdfsCopy(m_handler, src_path.c_str(), local_handler, dest_path.c_str());
    hdfsDisconnect(local_handler);
    return ret;
}
