#include "resender.h"

/*
 * implementation of resender class
 * author: Jiri Zahradnik
 * date: spring 2017
 */


resender *hack;

static void sig_handler(int s){
    std::cerr << std::endl << "Handling signal " << s << std::endl;
    hack->m_run = false;
}

resender::resender(){
    memset(&m_config, 0, sizeof(cfg_t));
    m_run = false;
    m_run_mut.lock();
}

resender::~resender(){
}

bool resender::read_config(){
    std::ifstream file;
    file.open("radar.conf");
    
    std::string line;
    std::string type;
    std::string value;
    
    std::vector<std::string> v;
    
    if(file.is_open()){
        while(getline(file, line)){
            if(line[0] == '#'){
                continue;
            }
            
            v = split(line, '=');
            if(v.size() < 2){
                continue;
            }
            
            if(v[0] == "PLANE_TYPE"){
                if(v[1] == "A320"){
                    m_config.plane_type = plane_type_e::A320;
                }
                else if(v[1] == "B747"){
                    m_config.plane_type = plane_type_e::B747;
                }
                else if(v[1] == "C130"){
                    m_config.plane_type = plane_type_e::C130;
                }
                else{
                    m_config.plane_type = plane_type_e::GENERIC;
                }
            }
            else if(v[0] == "FLARE_INIT_HEIGHT"){
                m_config.flare_init_height = std::stod(v[1]);
            }
            else if(v[0] == "FLARE_HEIGHT"){
                m_config.flare_height = std::stod(v[1]);
            }
            else if(v[0] == "CTRL_PORT"){
                m_config.port = std::stod(v[1]);
            }
        }
        file.close();
    }
    else{
        std::cerr << "Failed to open config file: radar.conf" << std::endl;
        return true;
    }
    return false;
}

std::vector<std::string> resender::split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

bool resender::receiver(){
    /* I had to fuck over with signal handler to allow me to change object variable at SIGINT*/
    hack = this;
    signal(SIGINT, &sig_handler);
    signal(SIGABRT, &sig_handler);
    signal(SIGTERM, &sig_handler);
    
    if(0 > (m_s = socket(AF_INET, SOCK_DGRAM, 0))){
        std::cerr << "Failed to create socket" << std::endl;
        return true;
    }
    struct sockaddr_in addr;
    /* init to zeros */
    memset(&addr, 0, sizeof(struct sockaddr_in));
    /* ipv4 */
    addr.sin_family = AF_INET;
    /* ipv4: 127.0.0.1 */
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    /* connect to port defined in config file */
    addr.sin_port = htons(m_config.port);
    /* set timeout */
    struct timeval tv;
    /* 0 sec */
    tv.tv_sec = 0;
    /* 500 mili sec */
    tv.tv_usec = 500000;
    if (0 > setsockopt(m_s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv))){
        std::cerr << "Failed to set timeout" << std::endl;
        return true;
    }
    /* bind to socket */    
    if(0 > bind(m_s, (struct sockaddr *)&addr, sizeof(struct sockaddr_in))){
        std::cerr << "Failed to bind socket" << std::endl;
        return true;
    }

    /*
     * receiving double + 3 zero bytes
     * protocol bytes:
     * | 1 | 2 | 3 | 4 | n = sizeof(double) |
     */
    packet_t packet;
    memset(&packet, 0x0, sizeof(packet_t));
    struct sockaddr_storage src_addr;
    socklen_t src_addr_len = sizeof(src_addr);

    /* allow receiving and control loops to iterate */
    m_run = true;
    /* allow main control thread to run */
    m_run_mut.unlock();
    while(m_run){
        ssize_t count = recvfrom(m_s, &packet, sizeof(packet), 0, (struct sockaddr*)&src_addr, &src_addr_len);
        if (-1 == count){
            std::cerr << "Failed to get datagram " << std::endl;
        }else if (count > (ssize_t)sizeof(packet)){
            std::cerr << "datagram too large for buffer: truncated" << std::endl;
        }else{
            parser(packet);
        }
    }
    
    close(m_s);
    return false;
}

void resender::controller(){
    m_run_mut.lock();
    packet_t packet;
    memset(&packet, 0x0, sizeof(packet_t));
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long old_time = 1000000 * tv.tv_sec + tv.tv_usec;
    long long difference = 0;
    
    while(m_run){
        /* empty check */
        if(m_q.empty()){
            continue;
        }
        /* get altitude and pop first item */
        packet = m_q.pop();
        
        difference = abs(packet.timestamp - old_time);
        /* too old information, exiting */
        if(DELTA_TIME < difference){
            gettimeofday(&tv, NULL);
            old_time = 1000000 * tv.tv_sec + tv.tv_usec;
            continue;
        }
        gettimeofday(&tv, NULL);
        old_time = 1000000 * tv.tv_sec + tv.tv_usec;
        std::cout << packet.altitude << std::endl;
        paudio::initSound();

        
        /* flare hold altitude */
        if(m_config.flare_height >= packet.altitude){
            paudio::setFreq(300);
            sleep(4);
        } else {
            paudio::setFreq(300);
            usleep(packet.altitude * 25000);

            paudio::setFreq(0);
            usleep(packet.altitude * 25000);
        }
        paudio::beep(523, 200);
        paudio::stopSound();
    }
}

bool resender::parser(packet_t packet){
    /* push data into Q */
    m_q.push(packet);

    return false;
}