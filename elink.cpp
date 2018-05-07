#include "elink.h"
#include <QDebug>

void Elink::usb_send(char *write_buf, int size, ELINK_CMD cmd)
{
    int Dev_num;
    int ret;
    Dev_num = USBScanDev(1);
    ret = USBOpenDev(0);
    if(ret == 1)
        qDebug()<<"打开设备失败";
    else
        ret = USBBulkWriteData(0,EP1_OUT,write_buf,size,500,cmd);

}

void Elink::usb_recive(char *read_buf, int size, ELINK_CMD cmd)
{
    int Dev_num;
    int ret;
    Dev_num = USBScanDev(1);
    ret = USBOpenDev(0);
    if(ret == 1)
        qDebug()<<"打开设备失败";
    else
        ret = USBBulkReadData(0,EP1_IN,read_buf,size,500);

}

bool Elink::check_device()
{
    char write_buf[EP1_OUT_SIZE];
    char read_buf[EP1_IN_SIZE];
    memset(write_buf,0,EP1_OUT_SIZE);
    memset(read_buf,0,EP1_IN_SIZE);

    usb_send(write_buf,EP1_OUT_SIZE,START);
    usb_recive(read_buf,EP1_IN_SIZE,START);
    if(read_buf[4])
    {
       return true;
    }else
    {
        return false;
    }

}

bool Elink::get_eeprom_data(char *read_buf, int size)
{

    char write_buf[EP1_OUT_SIZE];
    char read_buf_r[EP1_IN_SIZE];
    memset(write_buf,0,EP1_OUT_SIZE);
    memset(read_buf_r,0,EP1_IN_SIZE);

    int index = 0;
    for(int i = 0; i < size; i++)
    {
        write_buf[index] = i;
        write_buf[index + 1] = 0;
        index = index + 2;
        if(index == 32)
        {
            usb_send(write_buf,index,READ_RAM1);
            usb_recive(read_buf_r,EP1_IN_SIZE,READ_RAM1);
            for(int j = 0; j < 16;j++)
            {
                read_buf[j+i-15] = read_buf_r[4+j*2];
                //qDebug()<<QString("%1o").arg(read_buf_r[4+j*2]&0xff,2,16);
            }
            index = 0;
        }
    }
    if(index != 0)
    {

        usb_send(write_buf,index,READ_RAM1);
        usb_recive(read_buf_r,EP1_IN_SIZE,READ_RAM1);
        for(int j = 0; j < index/2;j++)
        {
            read_buf[size - size%16+j] = read_buf_r[4+j*2];
            //qDebug()<<QString("%1o").arg(read_buf_r[4+j*2]&0xff,2,16);
        }
    }


}

char Elink::put_eeprom_data(char *write_buf, int size)
{

    int count = 0;
    int index = 0;
    char write_buf_w[EP1_OUT_SIZE];
    char read_buf[EP1_IN_SIZE];
    memset(write_buf_w,0,EP1_OUT_SIZE);
    memset(read_buf,0,EP1_IN_SIZE);

    usb_send(write_buf_w,index,VERIFY_RAM1);
    usb_recive(read_buf,EP1_IN_SIZE,VERIFY_RAM1);

    for(int i = 0;i < 32;i++)
    {
        write_buf_w[count] = index;
        write_buf_w[count+1] = 0;
        count = count + 2;
        index++;
        if(count == 32)
        {
            usb_send(write_buf_w,32,READ_RAM1);
            usb_recive(read_buf,EP1_IN_SIZE,READ_RAM1);
            count = 0;
            for(int j = 0;j < 16;j++)
            {
                if(read_buf[4+j*2] != 0)
                {
                    return -1;// erase fail
                }
            }
        }
    }

    index =0;
    count =0;

    for(int i= 0; i < 32;i++)
    {
        write_buf_w[count] = index;
        write_buf_w[count+1] = write_buf[index];
        count = count + 2;
        index++;
        if(count == 32)
        {
            usb_send(write_buf_w,32,PROGRAM_RAM1);
            usb_recive(read_buf,EP1_IN_SIZE,PROGRAM_RAM1);
            count = 0;
        }
    }
    index = 0;
    count = 0;
    for(int i = 0;i < 32;i++)
       {
           write_buf_w[count] = index;
           write_buf_w[count+1] = 0;
           count = count + 2;
           index++;
           if(count == 32)
           {
               usb_send(write_buf_w,32,READ_RAM1);
               usb_recive(read_buf,EP1_IN_SIZE,READ_RAM1);
               count = 0;
               for(int j = 0;j < 16;j++)
               {
                   if((unsigned char)read_buf[4+j*2] !=(unsigned char) write_buf[j+index-16])
                   {
                       return -2; //write faile
                   }
               }
           }
       }
    return 0;//succeed



}

Elink::Elink()
{
 QLibrary lib("USB_Driver.DLL");
 lib.load();
 USBScanDev = (FUN1)lib.resolve("USBScanDev");
 USBOpenDev = (FUN2)lib.resolve("USBOpenDev");
 USBCloseDev = (FUN3)lib.resolve("USBCloseDev");
 USBBulkWriteData = (FUN4)lib.resolve("USBBulkWriteData");
 USBBulkReadData = (FUN5)lib.resolve("USBBulkReadData");
 USBIntWriteData = (FUN6)lib.resolve("USBIntWriteData");
 USBIntReadData = (FUN7)lib.resolve("USBIntReadData");
 USBCtrlData = (FUN8)lib.resolve("USBCtrlData");

}
