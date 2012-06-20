/*
 * xmodem-at91.c
 *
 * A simple program using xmodem/1kxmode upload file to at91rm9200 based board.
 * Created by (C) Copyright 2004
 * Linhang.Zhang, Jilin University of PR.China, linxing@jlu.edu.cn.
 *
 *************************************************************************************
 *
 * Modified 01-Feb-2005 (C)Copyright 2005
 * Marco Cavallini, www.KoanSoftware.com - ITALY, m.cavallini@koansoftware.com
 * - edited indentations and changed break usage in switch statement.
 * - added "\r" to printf
 *
 * - build with 
 *   gcc sx-at91.c -o sx-at91
 *
 * - Howto use this program with minicom/xminicom and AT91 
 *   start minicom or xminicom
 *   edit Options / File transfer protocol, 
 *   add a name (for example J) like the following example
 *
 *   |     Name             Program                 Name U/D FullScr IO-Red. Multi  |
 *   | A  zmodem     /usr/bin/sz -vv -b              Y    U    N       Y       Y    |
 *   | B  ymodem     /usr/bin/sb -vv                 Y    U    N       Y       Y    |
 *   | C  xmodem     /usr/bin/sx -vv                 Y    U    N       Y       N    |
 *   | D  zmodem     /usr/bin/rz -vv -b -E           N    D    N       Y       Y    |
 *   | E  ymodem     /usr/bin/rb -vv                 N    D    N       Y       Y    |
 *   | F  xmodem     /usr/bin/rx -vv                 Y    D    N       Y       N    |
 *   | G  kermit     /usr/bin/kermit -i -l %l -s     Y    U    Y       N       N    |
 *   | H  kermit     /usr/bin/kermit -i -l %l -r     N    D    Y       N       N    |
 *   | I  ascii      /usr/bin/ascii-xfr -dsv         Y    U    N       Y       N    |
 *   | J  at91       /home/koan/xmodem/xs-at91       Y    U    Y       N       N    |
 *   | K    -                                                                       |
 *   | L    -                                                                       |
 *
 *   save and use it selecting at91 protocol when you start an Xmodem upload to AT91
 *
 *************************************************************************************
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include     <stdio.h>
#include     <stdlib.h>
#include     <unistd.h>
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>
#include     <termios.h>
#include     <errno.h>
#include     <time.h>

/*
Xmodem Frame form: <SOH><blk #><255-blk #><--128 data bytes--><CRC hi><CRC lo>
*/

#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CRC_CHR	'C'
#define XMODEM_CRC_SIZE 2       /* Crc_High Byte + Crc_Low Byte */
#define XMODEM_FRAME_ID_SIZE 2  /* Frame_Id + 255-Frame_Id */
#define XMODEM_DATA_SIZE_SOH 128        /* for Xmodem protocol */
#define XMODEM_DATA_SIZE_STX 1024       /* for 1K xmodem protocol */
#define USE_1K_XMODEM 0         /* 1 for use 1k_xmodem 0 for xmodem */

#if (USE_1K_XMODEM)
#define XMODEM_DATA_SIZE 	XMODEM_DATA_SIZE_STX
#define XMODEM_HEAD		XMODEM_STX
#else
#define XMODEM_DATA_SIZE 	XMODEM_DATA_SIZE_SOH
#define XMODEM_HEAD 		XMODEM_SOH
#endif
/*********/

#define SERIAL_DEVICE "/dev/ttyS0"
#define MYBAUDRATE B115200

/***************SUB PROGRAM*******/
unsigned short GetCrc16(char *ptr, unsigned short count)
{
    unsigned short crc, i;

    crc = 0;
    while (count--) {
        crc = crc ^ (int)*ptr++ << 8;

        for (i = 0; i < 8; i++) {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        }
    }

    return (crc & 0xFFFF);
}

/*******************************/
int Initial_SerialPort(void)
{
    int fd;

    struct termios options;

    fd = open(SERIAL_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        /*
         * open error!
         */
        perror("Can't open serial port!");
        return -1;
    }

    /*
     * Get the current options for the port...
     */
    tcgetattr(fd, &options);

    /*
     * Set the baud rates to BAUDRATE...
     */
    cfsetispeed(&options, MYBAUDRATE);
    cfsetospeed(&options, MYBAUDRATE);
    tcsetattr(fd, TCSANOW, &options);
    if (0 != tcgetattr(fd, &options)) {
        perror("SetupSerial 1");
        return -1;
    }

    /*
     * 8bit Data,no partity,1 stop bit...
     */
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcflush(fd, TCIFLUSH);

        /***Choosing Raw Input*/
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;

    /*
     * Set the new options for the port...
     */
    if (0 != tcsetattr(fd, TCSANOW, &options)) {
        perror("SetupSerial error");
        return -1;
    }

    return fd;
}

/******************************/
void ClearReceiveBuffer(int fd)
{
    unsigned char tmp;

    while ((read(fd, &tmp, 1)) > 0) ;

    return;
}

/********************************/
int main(int argc, char *argv[])
{
    int fd;

    char *data_file_name;

    char packet_data[XMODEM_DATA_SIZE];

    char frame_data[XMODEM_DATA_SIZE + XMODEM_CRC_SIZE + XMODEM_FRAME_ID_SIZE +
                    1];
    FILE *datafile;

    int complete, retry_num, pack_counter, read_number, write_number, i;

    unsigned short crc_value;

    unsigned char ack_id;

    printf("sx-at91 started...\r\n");

    /*
     * open serial port1 
     */
    if ((fd = Initial_SerialPort()) == -1)
        return -1;

    data_file_name = argv[1];

    if ((datafile = fopen(data_file_name, "rb")) == NULL) {
        perror("Can't open file!");
        return -1;
    }

        /*******************************/

    pack_counter = 0;
    complete = 0;
    retry_num = 0;
    ClearReceiveBuffer(fd);

    while ((read(fd, &ack_id, 1)) <= 0) ;

    printf("%c\r\n", ack_id);
    ack_id = XMODEM_ACK;
    while (!complete) {
        switch (ack_id) {
        case XMODEM_ACK:
            retry_num = 0;
            pack_counter++;
            read_number =
                fread(packet_data, sizeof (char), XMODEM_DATA_SIZE, datafile);
            if (read_number > 0) {
                if (read_number < XMODEM_DATA_SIZE_SOH) {

                    printf("Start filling the last frame!\r\n");
                    for (; read_number < XMODEM_DATA_SIZE; read_number++)
                        packet_data[read_number] = 0x00;
                }
                frame_data[0] = XMODEM_HEAD;
                frame_data[1] = (char)pack_counter;
                frame_data[2] = (char)(255 - frame_data[1]);

                for (i = 0; i < XMODEM_DATA_SIZE; i++)
                    frame_data[i + 3] = packet_data[i];

                crc_value = GetCrc16(packet_data, XMODEM_DATA_SIZE);
                frame_data[XMODEM_DATA_SIZE_SOH + 3] =
                    (unsigned char)(crc_value >> 8);
                frame_data[XMODEM_DATA_SIZE_SOH + 4] =
                    (unsigned char)(crc_value);
                write_number = write(fd, frame_data, XMODEM_DATA_SIZE_SOH + 5);
                printf("waiting for ACK,%d,%d,...", pack_counter, write_number);
                while ((read(fd, &ack_id, 1)) <= 0) ;

                if (ack_id == XMODEM_ACK)
                    printf("Ok!\r\n");
                else
                    printf("Error!\r\n");
            } else {
                ack_id = XMODEM_EOT;
                complete = 1;
                printf("Waiting for complete ACK ...");

                while (ack_id != XMODEM_ACK) {
                    ack_id = XMODEM_EOT;
                    write_number = write(fd, &ack_id, 1);
                    while ((read(fd, &ack_id, 1)) <= 0) ;
                }
                printf("OK\r\n");

                printf("Sending file complete\r\n");
            }
            break;

        case XMODEM_NAK:
            if (retry_num++ > 10) {
                printf("Retry too many times,Quit!\r\n");
                complete = 1;
            } else {
                write_number = write(fd, frame_data, XMODEM_DATA_SIZE + 5);
                printf("Retry for ACK,%d,%d...", pack_counter, write_number);
                while ((read(fd, &ack_id, 1)) <= 0) ;

                if (ack_id == XMODEM_ACK)
                    printf("OK\r\n");
                else
                    printf("Error!\r\n");
            }
            break;

        default:
            printf("Fatal Error!\r\n");
            complete = 1;
            break;
        }

    }

    fclose(datafile);
    close(fd);

    return 0;
}
