#include "main.h"
#include "debug.h"
#include "dbgu.h"
#include "linux_tags.h"
#include "dataflash.h"
#include "nandflash.h"

#if defined(CONFIG_AT91SAM9X5EK)
#include "onewire_info.h"
#endif

#ifdef CONFIG_LOAD_LINUX

#ifdef WINCE

#ifdef CONFIG_AT91SAM9260EK
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9261EK
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9263EK
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9RLEK
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9G10EK
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9G20EK
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9G45EK
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=128M console=ttyS0,115200 mtdparts=atmel_nand:4M(bootstrap),60M(rootfs),-(spare) root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=128M console=ttyS0,115200 mtdparts=atmel_nand:4M(bootstrap),60M(rootfs),-(spare) root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=128M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9M10EK
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 mtdparts=atmel_nand:4M(bootstrap),60M(rootfs),-(spare) root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 mtdparts=atmel_nand:4M(bootstrap),60M(rootfs),-(spare) root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9G45EKES
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=128M console=ttyS0,115200 mtdparts=atmel_nand:4M(bootstrap),60M(rootfs),-(spare) root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=128M console=ttyS0,115200 mtdparts=atmel_nand:4M(bootstrap),60M(rootfs),-(spare) root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=128M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif
#ifdef CONFIG_AT91SAM9M10EKES
#ifdef CONFIG_DATAFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 mtdparts=atmel_nand:4M(bootstrap),60M(rootfs),-(spare) root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_NANDFLASH
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 mtdparts=atmel_nand:4M(bootstrap),60M(rootfs),-(spare) root=/dev/mtdblock1 rw rootfstype=jffs2"
#endif
#ifdef CONFIG_SDCARD
#define LINUX_KERNEL_ARG_STRING "mem=64M console=ttyS0,115200 root=/dev/mmcblk0p2 rootdelay=2"
#endif
#endif

#endif                          /* WINCE */

#define BIN_LOAD_ADDR (OS_MEM_BANK + 0x2000000)
#define IMAGE_MAGIC 0x27051956

size_t strlen(const char *str);

char *strcpy(char *dst, const char *src);

void *memcpy(void *dst, const void *src, size_t cnt);

extern inline void disable_icache(void);

extern inline void disable_dcache(void);

extern inline void disable_irq(void);

extern inline void flush_idcache(void);

typedef struct image_header {
    unsigned long ih_magic;
    unsigned long ih_hcrc;
    unsigned long ih_time;
    unsigned long ih_size;
    unsigned long ih_load;
    unsigned long ih_ep;
    unsigned long ih_dcrc;
    unsigned char ih_os;
    unsigned char ih_arch;
    unsigned char ih_type;
    unsigned char ih_comp;
    unsigned char ih_name[32];
} image_header_t;

static long ntohl(unsigned long dat)
{
    return (((dat & 0x000000FF) << 24)
            | ((dat & 0x0000FF00) << 8)
            | ((dat & 0xFF000000) >> 24)
            | ((dat & 0x00FF0000) >> 8)
        );
}

static void setup_cmdline_tag(struct tag *tag, char *commandline)
{
    char *p;

    //if (!commandline)
    //    return;

    /*
     * eat leading white space 
     */
    for (p = commandline; *p == ' '; p++) ;

    /*
     * skip non-existent command lines so the kernel will still
     * * use its default command line.
     */
    if (*p == '\0')
        return;

    tag->hdr.tag = ATAG_CMDLINE;
    tag->hdr.size = (sizeof (struct tag_header) + strlen(p) + 1 + 4) >> 2;
    strcpy(tag->u.cmdline.cmdline, p);
}

void clean_environment()
{
#ifndef WINCE
    /*
     * Disable ARM Core interrupt 
     */
    disable_irq();
    /*
     * Turen off I/D cache 
     */
    disable_icache();
    disable_dcache();
    /*
     * Flush I/D cache 
     */
    flush_idcache();
#endif
}

void setup_tags()
{
    struct tag *tag = (struct tag *)(OS_MEM_BANK + 0x100);

    /*
     * Start tag
     */
    tag->hdr.tag = ATAG_CORE;
    tag->hdr.size = tag_size(tag_core);
    tag->u.core.flags = 0;
    tag->u.core.pagesize = 0;
    tag->u.core.rootdev = 0;
    tag = tag_next(tag);

    /*
     * Mem tag
     */
    tag->hdr.tag = ATAG_MEM;
    tag->hdr.size = tag_size(tag_mem32);
    tag->u.mem.start = OS_MEM_BANK;
    tag->u.mem.size = OS_MEM_SIZE;
    tag = tag_next(tag);

    /*
     * Command line tag
     */
    setup_cmdline_tag(tag, LINUX_KERNEL_ARG_STRING);
    tag = tag_next(tag);

#if defined(CONFIG_AT91SAM9X5EK)
    /*
     * System Rev tag
     */
    tag->hdr.tag = ATAG_REVISION;
    tag->hdr.size = tag_size(tag_revision);
    tag->u.revision.rev = get_sys_rev();
    tag = tag_next(tag);

    /*
     * Board Serial tag
     */
    tag->hdr.tag = ATAG_SERIAL;
    tag->hdr.size = tag_size(tag_serialnr);
    tag->u.serialnr.low = get_sys_sn();
    tag->u.serialnr.high = 0;
    tag = tag_next(tag);
#endif

    /*
     * end tag
     */
    tag->hdr.tag = ATAG_NONE;
    tag->hdr.size = 0;
}

void LoadApp(char *appfilename) {
	void (*theApp)(void);
    unsigned int load_SDCard(void *dst, char *os_image_name);

    if (load_SDCard((void *)0x20000000,appfilename)==0) {
		return;
	}

    theApp = (void (*)(void))0x20000000;
    dbg_log(4, "Run %s at %x\n\r",appfilename,theApp);
	theApp();
}

void LoadLinux(char *image_name)
{
    unsigned long ep, load_addr, len;
    unsigned int load_SDCard(void *dst, char *os_image_name);
    void (*theKernel) (int zero, int arch, unsigned int params);

    image_header_t *hdr;

    if (load_SDCard((void *)JUMP_ADDR,image_name)==0) {
		return;
	}

    hdr = (image_header_t *) JUMP_ADDR;
    if (ntohl(hdr->ih_magic) != IMAGE_MAGIC) {
        dbg_log(0, "*** Bad Image Magic Number found! %d\n\r",
                ntohl(hdr->ih_magic));
        return;
    }

    len = ntohl(hdr->ih_size);
    load_addr = ntohl(hdr->ih_load);
    ep = ntohl(hdr->ih_ep);
    dbg_log(0, "Image size: %d, load_addr: %x, ep: %x\n\r", len, load_addr, ep);

    theKernel = (void (*)(int, int, unsigned int))ep;

    dbg_log(1,
            "relocating linux kernel, dst: %x, src: %x, len: %d, machid: %d\n\r",
            load_addr, (unsigned long)JUMP_ADDR + sizeof (image_header_t), len,
            MACH_TYPE);
    memcpy((void *)load_addr,
           (void *)((unsigned long)JUMP_ADDR + sizeof (image_header_t)), len);
    dbg_log(1, "... %d bytes data transferred!\n\r", len);

    setup_tags();

	// Carica il Device tree
    if (load_SDCard((void *)OS_MEM_BANK + 0x100,"ariag25.dtb")==0) {
		return;
	}

    dbg_log(1, "\n\rStarting linux kernel ..., machid: %d, tags: %x\n\r\n\r",
            MACH_TYPE, (OS_MEM_BANK + 0x100));

    theKernel(0, MACH_TYPE, (unsigned int)(OS_MEM_BANK + 0x100));

    /*
     * never reach here 
     */
    //return;
}

#endif
