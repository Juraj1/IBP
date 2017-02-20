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

#include "threadSafety.h"

typedef enum class plane_type_e{
    A320 = 0,
    B747,
    C130,
    GENERIC
}plane_type_t;

typedef union {
    unsigned long rcv;
    double d_rcv;
}alt_t;

typedef struct config_t{
    plane_type_t plane_type;
    double flare_init_height;
    double flare_height;
    uint16_t port;
}cfg_t;

class resender{
public:
    /* variables */
    
    threadSafety::queue_t<double> m_q; //< @brief Queue for event push
    std::mutex m_run_mut; //< @brief Mutex to lock controller thread
    cfg_t m_config; //< @brief Structure for configuration file
    bool m_run; //< @brief stop flag
    int m_s; //< @brief socket


    /* methods */
    resender();
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
    bool parser(void *);
    
protected:
    /* variables */

    /* methods */
};

#endif /* __RESENDER_H__ */