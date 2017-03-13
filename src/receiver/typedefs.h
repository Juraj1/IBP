#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

/*
 * definition of resender class and required data types
 * author: Jiri Zahradnik
 * date:   spring 2017
 */

/* flag definitions */
#define ERR_OK 0x00
#define ERR_RADAR_HW 0x01
#define ERR_RADAR_SW 0x02
#define ERR_CONNECTION 0x03

typedef enum class plane_type_e{
    A320 = 0,
    B747,
    C130,
    GENERIC
}plane_type_t;

typedef struct __attribute__ ((packed)) {
    uint8_t flags;
    double altitude;
    unsigned long long timestamp;
    uint64_t reserved;
    uint8_t end_byte;
}packet_t;

typedef struct config_t{
    plane_type_t plane_type;
    double flare_init_height;
    double flare_height;
    uint16_t port;
}cfg_t;

#endif /* __TYPEDEFS_H__ */