#ifndef __RESENDER_H__
#define __RESENDER_H__

/*
 * definition of resender class and required data types
 * author: Jiri Zahradnik
 * date:   spring 2017
 */

#include <queue>
#include <mutex>
#include <thread>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <signal.h>
#include <memory>

/* socket interface */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ioctl.h>

/* time */
#include <sys/time.h>

#include "threadSafety.h"
#include "paudio.h"
#include "typedefs.h"

#define OFFSET_ALT_PART (1)
#define OFFSET_TIMESTAMP_PART (OFFSET_ALT_PART + sizeof(double))
#define OFFSET_RESERVED_PART (OFFSET_TIMESTAMP_PART + 8)
#define OFFSET_END (OFFSET_RESERVED_PART + 1)

#define DELTA_TIME 150000 /* [u sec] */


class resender{
public:
    /* variables */
    
    threadSafety::queue_t<packet_t> m_q; //< @brief Queue for event push
    std::mutex m_run_mut; //< @brief Mutex to lock controller thread
    cfg_t m_config; //< @brief Structure for configuration file
    bool m_run; //< @brief stop flag
    int m_s; //< @brief socket

    /* methods */
    resender();
    ~resender();
    /*
     * @brief reads config file
     * @return true on error
     */
    bool read_config();
    
    /*
     * @brief string splitter for read_config()
     * @param s Reference to the string
     * @param delim Delimiter by which we are cutting the string
     * @return Vector with tokens
     */
    std::vector<std::string> split(const std::string &s, char delim);
    
    /*
     * @brief reciever from server, runs in 1st thread
     * @return true on error
     */
    bool receiver();
    
    /* @brief controlls output, runs in 2nd thread */
    void controller();

    /*
     * @brief parses message and puts it in the Q
     * @param buff Buffer with stored message to parse
     * @return true on error
     */
    bool parser(packet_t);
    
protected:
    /* variables */

    /* methods */
};

#endif /* __RESENDER_H__ */