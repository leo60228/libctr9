#include <3ds9/ctr_sd_interface.h>
#include <3ds9/sdmmc/sdmmc.h>
#include <string.h>


static const ctr_io_interface sd_base = 
{
	ctr_sd_interface_read,
	ctr_sd_interface_write,
	ctr_sd_interface_read_sector,
	ctr_sd_interface_write_sector,
	ctr_sd_interface_disk_size,
	ctr_sd_interface_sector_size
};

int ctr_sd_interface_initialize(ctr_sd_interface *io)
{
	io->base = sd_base;
	InitSD();
	return SD_Init();
}

void ctr_sd_interface_destroy(ctr_sd_interface *io)
{
	
}

int ctr_sd_interface_read(void *ctx, void *buffer, size_t buffer_size, size_t position, size_t count)
{
	if (count)
	{
		uint8_t *dest = buffer;
		uint8_t buf[0x200u];
		const size_t base_sector = position / 0x200u;
		const size_t start_location = position % 0x200u;

		size_t bytes_read = 0;
		size_t sectors_read = 0;

		//read first sector to extract the right number of bytes from it
		sdmmc_nand_readsectors(base_sector, ++sectors_read, buf);

		const size_t readable = 0x200u - start_location;
		bytes_read += readable < count ? readable : count;

		memcpy(dest, &buf[start_location], bytes_read);

		//read all sectors until the last one
		const size_t mid_sectors = (count-bytes_read)/0x200u;
		if (mid_sectors)
		{
			sdmmc_sdcard_readsectors(base_sector + sectors_read, mid_sectors, dest + bytes_read);
			sectors_read += mid_sectors;
			bytes_read += mid_sectors * 0x200u;
		}

		if (bytes_read != count)
		{
			//read last sector to extract the right number of bytes from it
			sdmmc_sdcard_readsectors(base_sector + sectors_read, 1, buf);
			memcpy(dest + bytes_read, buf, count - bytes_read);
		}
	}
	return 0;
}

int ctr_sd_interface_write(void *ctx, const void *buffer, size_t buffer_size, size_t position)
{
	if (buffer_size)
	{
		const uint8_t *source = buffer;
		uint8_t buf[0x200u];
		const size_t base_sector = position / 0x200u;
		const size_t start_location = position % 0x200u;

		size_t bytes_written = 0;
		size_t sectors_written = 0;

		//read first sector to extract the right number of bytes from it
		sdmmc_sdcard_readsectors(base_sector, ++sectors_written, buf);

		const size_t writeable = 0x200u - start_location;
		bytes_written += writeable < buffer_size ? writeable : buffer_size;

		memcpy(buf + start_location, source, bytes_written);
		sdmmc_sdcard_writesectors(base_sector, sectors_written, buf);

		const size_t mid_sectors = (buffer_size-bytes_written)/0x200u;
		//read all sectors until the last one
		if (mid_sectors)
		{
			sdmmc_sdcard_writesectors(base_sector + sectors_written, mid_sectors, source + bytes_written);
			sectors_written += mid_sectors;
			bytes_written += mid_sectors * 0x200u;
		}

		if (bytes_written != buffer_size)
		{
			//read last sector to extract the right number of bytes from it
			sdmmc_sdcard_readsectors(base_sector + sectors_written, 1, buf);
			memcpy(buf, source + bytes_written, buffer_size - bytes_written);
			sdmmc_sdcard_writesectors(base_sector + sectors_written, 1, buf);
		}
	}
	return 0;
}

int ctr_sd_interface_read_sector(void *ctx, void *buffer, size_t buffer_size, size_t sector, size_t count)
{
	size_t read_size = (buffer_size / 512) < count ? buffer_size / 512 : count;
	int res = sdmmc_sdcard_readsectors(sector, read_size, (uint8_t*) buffer);
	return res;
}

int ctr_sd_interface_write_sector(void *ctx, const void *buffer, size_t buffer_size, size_t sector)
{
	size_t write_size = (buffer_size / 512);
	int res = sdmmc_sdcard_writesectors(sector, write_size, (const uint8_t*)buffer);
	return res;
}

size_t ctr_sd_interface_disk_size(void *ctx)
{
	return 0;
}

size_t ctr_sd_interface_sector_size(void *ctx)
{
	return 512;
}

