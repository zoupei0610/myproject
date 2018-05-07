#ifndef ELINK_H
#define ELINK_H

#include <QLibrary>

enum ELINK_CMD{
    PASSWD,
    START,
    END,
    OPEN,
    CLOSE,

    AP_WRITE,
    AP_READ,

    DP_WRITE,
    DP_READ,

    LOAD_HEX,
    READ_RAM,
    READ_RAM1,
    READ_OTP,
    READ_FLASH,
    READ_PATCH,

    MEMWRITE,
    MEMREAD,
    REGWRITE,
    REGREAD,

    MRESET,

    PROGRAM_RAM,
    PROGRAM_RAM1,
    PROGRAM_OTP,
    PROGRAM_PATCH,
    PROGRAM_FLASH,
    PROGRAM_XXX,

    VERIFY_RAM,
    VERIFY_RAM1,
    VERIFY_OTP,
    VERIFY_PATCH,
    VERIFY_FLASH,
    VERIFY_XXX,

};


#define EP0		0x00
#define EP1_IN	0x81	//wMaxPacketSize:   16	Int
#define EP1_OUT	0x01	//wMaxPacketSize:   16	Int
#define EP2_IN	0x82	//wMaxPacketSize:   64	Bulk
#define EP2_OUT	0x02	//wMaxPacketSize:   64	Bulk


#define	EP1_OUT_SIZE	60
#define	EP1_IN_SIZE	64


class Elink
{
private:
    typedef int (*FUN1)(int NeedInit);
    typedef int (*FUN2)(int DevIndex);
    typedef int (*FUN3)(int DevIndex);
    typedef int (*FUN4)(unsigned int nBoardID,int pipenum,char *sendbuffer,int len,int waittime,ELINK_CMD cmd);
    typedef int (*FUN5)(unsigned int nBoardID,int pipenum,char *readbuffer,int len,int waittime);
    typedef int (*FUN6)(unsigned int nBoardID,int pipenum,char *sendbuffer,int len,int waittime);
    typedef int (*FUN7)(unsigned int nBoardID,int pipenum,char *readbuffer,int len,int waittime);
    typedef int (*FUN8)(unsigned int nBoardID,int requesttype,int request,int value, int index, char *bytes, int size,int waittime);

    FUN1 USBScanDev;
    FUN2 USBOpenDev;
    FUN3 USBCloseDev;
    FUN4 USBBulkWriteData;
    FUN4 USBBulkWriteCmd;
    FUN5 USBBulkReadData;
    FUN6 USBIntWriteData;
    FUN7 USBIntReadData;
    FUN8 USBCtrlData;

public:
    void usb_send(char *write_buf,int size,ELINK_CMD  cmd);
    void usb_recive(char *read_buf,int size,ELINK_CMD  cmd);
    bool check_device(void);
    bool get_eeprom_data(char *read_buf,int size);
    char put_eeprom_data(char *write_buf,int size);


public:
    Elink();



};

#endif // ELINK_H
