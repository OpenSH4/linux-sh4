/*
 * arch/sh/boards/mach-b2037/setup.c
 *
 * Copyright (C) 2011 STMicroelectronics Limited
 * Author: Srinivas Kandagatla (srinivas.kandagatla@st.com)
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/phy.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/tm1668.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/physmap.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_gpio.h>
#include <linux/spi/flash.h>
#include <linux/stm/pci-glue.h>
#include <linux/stm/nand.h>
#include <linux/stm/emi.h>
#include <linux/stm/platform.h>
#include <linux/stm/stx7108.h>
#include <linux/stm/sysconf.h>
#include <asm/irq-ilc.h>

/*
 *
 * -----------------------------------------------------------------
 *			Lille Mode Pins:
 *
 * -----------------------------------------------------------------
 *		    SPI-NOR	  NOR   NAND
 *		     (11010)	 (X16)
 * -----------------------------------------------------------------
 *	JF8 1 (M2)	OFF	ON	OFF
 *	JF4 2 (M3)	ON	OFF	ON
 *	JF5 1 (M4)	OFF	ON	ON
 *	    2 (M5)	ON	OFF	OFF
 *	JF9 2 (M6)	ON	ON	ON
 * -----------------------------------------------------------------
 *	JF3 (NAND CS)	CSB	CSA
 * -----------------------------------------------------------------
 *
 *
 */




#define B2037_PIO_POWER_ON_ETHERNET stm_gpio(3, 6)
#define B2037_GPIO_SPI_FLASH_WP stm_gpio(2, 3)
#define B2037_GPIO_FLASH_HOLD stm_gpio(2, 2)
#define B2037_GPIO_FLASH_WP stm_gpio(12, 2)
#define B2037_GPIO_SYS_CONFIG_CS stm_gpio(12, 7)


static void __init b2037_setup(char **cmdline_p)
{
	printk(KERN_INFO "STMicroelectronics B2037(7108asLille) board initialisation\n");

	stx7108_early_device_init();

	stx7108_configure_asc(3, &(struct stx7108_asc_config) {
			.routing.asc3.txd = stx7108_asc3_txd_pio24_4,
			.routing.asc3.rxd = stx7108_asc3_rxd_pio24_5,
			.hw_flow_control = 0,
			.is_console = 1, });
	stx7108_configure_asc(1, &(struct stx7108_asc_config) {
			.hw_flow_control = 1, });
}


static struct tm1668_key b2037_front_panel_keys[] = {
	{ 0x00001000, KEY_UP, "Up (SWF2)" },
	{ 0x00800000, KEY_DOWN, "Down (SWF7)" },
	{ 0x00008000, KEY_LEFT, "Left (SWF6)" },
	{ 0x00000010, KEY_RIGHT, "Right (SWF5)" },
	{ 0x00000080, KEY_ENTER, "Enter (SWF1)" },
	{ 0x00100000, KEY_ESC, "Escape (SWF4)" },
};

static struct tm1668_character b2037_front_panel_characters[] = {
	TM1668_7_SEG_HEX_DIGITS,
	TM1668_7_SEG_HEX_DIGITS_WITH_DOT,
	TM1668_7_SEG_SEGMENTS,
};

static struct platform_device b2037_front_panel = {
	.name = "tm1668",
	.id = -1,
	.dev.platform_data = &(struct tm1668_platform_data) {
		.gpio_dio = stm_gpio(26, 2), /* FP_DA */
		.gpio_sclk = stm_gpio(26, 1), /* FP_CL */
		.gpio_stb = stm_gpio(2, 4), /* FP_CS */
		.config = tm1668_config_6_digits_12_segments,

		.keys_num = ARRAY_SIZE(b2037_front_panel_keys),
		.keys = b2037_front_panel_keys,
		.keys_poll_period = DIV_ROUND_UP(HZ, 5),

		.brightness = 8,
		.characters_num = ARRAY_SIZE(b2037_front_panel_characters),
		.characters = b2037_front_panel_characters,
		.text = "2037",
	},
};

static int b2037_phy_reset(void *bus)
{
	gpio_set_value(B2037_PIO_POWER_ON_ETHERNET, 1);
	gpio_set_value(B2037_PIO_POWER_ON_ETHERNET, 0);
	udelay(10000); /* 10 miliseconds is enough for everyone ;-) */
	gpio_set_value(B2037_PIO_POWER_ON_ETHERNET, 1);
	udelay(10000); /* 10 miliseconds is enough for everyone ;-) */

	return 1;
}


static struct stmmac_mdio_bus_data stmmac0_mdio_bus = {
	.bus_id = 0,
	.phy_reset = b2037_phy_reset,
	.phy_mask = 0,
};


/* NOR FLASH */
static struct mtd_partition b2037_nor_flash_parts[3] = {
	{
		.name = "NOR 1",
		.size = 0x00040000,
		.offset = 0x00000000,
	}, {
		.name = "NOR 2",
		.size = 0x00200000,
		.offset = 0x00040000,
	}, {
		.name = "NOR 3",
		.size = MTDPART_SIZ_FULL,
		.offset = 0x00240000,
	}
};

static struct physmap_flash_data b2037_nor_flash_data = {
	.width		= 1,
	.set_vpp	= NULL,
	.nr_parts	= ARRAY_SIZE(b2037_nor_flash_parts),
	.parts		= b2037_nor_flash_parts,
};

static struct platform_device b2037_nor_flash = {
	.name		= "physmap-flash",
	.id		= -1,
	.num_resources	= 1,
	.resource	= (struct resource[]) {
		{
			.start		= 0x00000000,
			.end		= 128*1024*1024 - 1,
			.flags		= IORESOURCE_MEM,
		}
	},
	.dev		= {
		.platform_data	= &b2037_nor_flash_data,
	},
};

/* Serial FLASH */
static struct platform_device b2037_serial_flash_bus = {
	.name           = "spi_gpio",
	.id             = 8,
	.num_resources  = 0,
	.dev            = {
		.platform_data =
		&(struct spi_gpio_platform_data) {
			.sck = stm_gpio(1, 6),
			.mosi = stm_gpio(2, 1),
			.miso = stm_gpio(2, 0),
			.num_chipselect = 1,
		}
	},
};

static struct mtd_partition b2037_serial_flash_parts[] = {
	{
		.name = "Serial 1",
		.size = 0x00400000,
		.offset = 0,
	}, {
		.name = "Serial 2",
		.size = MTDPART_SIZ_FULL,
		.offset = MTDPART_OFS_NXTBLK,
	},
};

static struct flash_platform_data b2037_serial_flash_data = {
	.name = "m25p80",
	.parts = b2037_serial_flash_parts,
	.nr_parts = ARRAY_SIZE(b2037_serial_flash_parts),
	.type = "n25q128",	/* Check device on individual board! */
};

static struct spi_board_info b2037_serial_flash[] =  {
	{
		.modalias       = "m25p80",
		.bus_num        = 8,
		.chip_select    = 0,
		.controller_data = (void *)stm_gpio(1, 7),
		.max_speed_hz   = 1000000,
		.platform_data  = &b2037_serial_flash_data,
		.mode           = SPI_MODE_0,
	},
};


/* NAND FLASH */
static struct mtd_partition b2037_nand_flash_parts[] = {
	{
		.name   = "NAND 1",
		.offset = 0,
		.size   = 0x00800000
	}, {
		.name   = "NAND 2",
		.offset = MTDPART_OFS_APPEND,
		.size   = MTDPART_SIZ_FULL
	},
};

static struct stm_nand_bank_data b2037_nand_flash_data = {
	.csn		= 1,
	.nr_partitions	= ARRAY_SIZE(b2037_nand_flash_parts),
	.partitions	= b2037_nand_flash_parts,
	.options	= NAND_NO_AUTOINCR || NAND_USE_FLASH_BBT,
	.timing_data = &(struct stm_nand_timing_data) {
		.sig_setup      = 10,           /* times in ns */
		.sig_hold       = 10,
		.CE_deassert    = 0,
		.WE_to_RBn      = 100,
		.wr_on          = 10,
		.wr_off         = 30,
		.rd_on          = 10,
		.rd_off         = 30,
		.chip_delay     = 30,           /* in us */
	},
	.emi_withinbankoffset	= 0,
};

static struct platform_device nand_device = {
	.name           = "stm-nand-emi",
	.dev.platform_data = &(struct stm_plat_nand_emi_data){
		.nr_banks       = 1,
		.banks          = &b2037_nand_flash_data,
		.emi_rbn_gpio   = -1,
	},
};

static struct platform_device *b2037_devices[] __initdata = {
	&b2037_front_panel,
	&b2037_serial_flash_bus,
	/* DISABLED FOR NOW */
	/* &b2037_nor_flash,
	&nand_device */
};

static int __init device_init(void)
{
	u32 bank1_start;
	u32 bank2_start;
	u32 bank3_start;
	u32 bank4_start;
	u32 boot_device;
	struct sysconf_field *sc;
	sc = sysconf_claim(SYS_STA_BANK1, 3, 2, 5, "ModePins");

	boot_device = sysconf_read(sc);

	bank1_start = emi_bank_base(1);
	bank2_start = emi_bank_base(2);
	bank3_start = emi_bank_base(3);
	bank4_start = emi_bank_base(4);

	BUG_ON(boot_device > 0xA);
	switch (boot_device) {
	case 0x0:
	case 0x5:
		/* Boot-from-NOR */
		pr_info("Configuring FLASH for boot-from-NOR\n");
		b2037_nor_flash.resource[0].start = 0x00000000;
		b2037_nor_flash.resource[0].end = bank1_start - 1;
		b2037_nand_flash_data.csn = 1;
		break;
	case 0xA:
		/* Boot-from-SPI */
		pr_info("Configuring FLASH for boot-from-SPI\n");
		b2037_nor_flash.resource[0].start = bank3_start;
		b2037_nor_flash.resource[0].end = bank4_start - 1;
		b2037_nand_flash_data.csn = 1;
		break;
	default:
		/* Boot-from-NAND */
		pr_info("Configuring FLASH for boot-from-NAND\n");
		b2037_nor_flash.resource[0].start = bank1_start;
		b2037_nor_flash.resource[0].end = bank2_start - 1;
		b2037_nand_flash_data.csn = 0;
		break;
	}

	gpio_request(B2037_GPIO_SYS_CONFIG_CS, "SYS_CONFIG_CS");
	gpio_direction_output(B2037_GPIO_SYS_CONFIG_CS, 0);
	/* Limit NOR FLASH to addressable range, regardless of actual EMI
	 * configuration! */
	if (b2037_nor_flash.resource[0].end -
	    b2037_nor_flash.resource[0].start > 0x4000000)
		b2037_nor_flash.resource[0].end =
			b2037_nor_flash.resource[0].start + 0x4000000-1;


	/* The "POWER_ON_ETH" line should be rather called "PHY_RESET",
	 * but it isn't... ;-) */
	gpio_request(B2037_PIO_POWER_ON_ETHERNET, "POWER_ON_ETHERNET");
	gpio_direction_output(B2037_PIO_POWER_ON_ETHERNET, 0);

	gpio_request(B2037_GPIO_FLASH_WP, "FLASH_WP");
	gpio_direction_output(B2037_GPIO_FLASH_WP, 1);

	gpio_request(B2037_GPIO_SPI_FLASH_WP, "SPI_FLASH_WP");
	gpio_direction_output(B2037_GPIO_SPI_FLASH_WP, 1);

	gpio_request(B2037_GPIO_FLASH_HOLD, "FLASH_HOLD");
	gpio_direction_output(B2037_GPIO_FLASH_HOLD, 1);

	/* Serial Flash */
	spi_register_board_info(b2037_serial_flash,
				ARRAY_SIZE(b2037_serial_flash));
	/* NIM */
	stx7108_configure_ssc_i2c(1, NULL);

	 /* HDMI */
	stx7108_configure_ssc_i2c(5, NULL);

	/* EEPROM, AV buffer(STV6440), Front Panel Micro & MII0 */
	stx7108_configure_ssc_i2c(6, NULL);


	stx7108_configure_lirc(&(struct stx7108_lirc_config) {
			.rx_mode = stx7108_lirc_rx_mode_ir, });

	stx7108_configure_usb(0);
	stx7108_configure_usb(1);
	stx7108_configure_usb(2);

	/* RMII with 25Mhz external clock */
	stx7108_configure_ethernet(0, &(struct stx7108_ethernet_config) {
			.mode = stx7108_ethernet_mode_rmii,
			.ext_clk = 1,
			.phy_bus = 0,
			.phy_addr = -1,
			.mdio_bus_data = &stmmac0_mdio_bus,
		});
	spi_register_board_info(b2037_serial_flash,
				ARRAY_SIZE(b2037_serial_flash));

	stx7108_configure_mmc(0);

	return platform_add_devices(b2037_devices,
			ARRAY_SIZE(b2037_devices));
}
arch_initcall(device_init);


static void __iomem *b2037_ioport_map(unsigned long port, unsigned int size)
{
	/* If we have PCI then this should never be called because we
	 * are using the generic iomap implementation. If we don't
	 * have PCI then there are no IO mapped devices, so it still
	 * shouldn't be called. */
	BUG();
	return NULL;
}

struct sh_machine_vector mv_b2037 __initmv = {
	.mv_name = "7108aslille_hdk",
	.mv_setup = b2037_setup,
	.mv_nr_irqs = NR_IRQS,
	.mv_ioport_map = b2037_ioport_map,
	STM_PCI_IO_MACHINE_VEC
};

