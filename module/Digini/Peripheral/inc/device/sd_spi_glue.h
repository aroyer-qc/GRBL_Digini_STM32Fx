#pragma once

#include "project_def.h"

#define SD_SPI_TICK_PERIOD                    1            // in ms
#define SD_SPI_PORT                            HAL_SPI1

/*----------------------------------------------------------------------*/
/* Power Control                                                          */
/*----------------------------------------------------------------------*/
#define SD_SPI_CARD_PWR_ON                    1
#define SD_SPI_CARD_PWR_OFF                    0
#define sd_spi_card_power( state )            // always on            //hal_GpioSetPinState( /* TBD */ , /* TBD */ )
#define sd_spi_chk_power()                    SD_SPI_CARD_PWR_ON        //hal_GpioGetPinState( /* TBD */ , /* TBD */ )

/*----------------------------------------------------------------------*/
/* Card-Select Controls                                                     */
/*----------------------------------------------------------------------*/
/* get spi resource &  sd card chip select */
#define sd_spi_select()                        hal_GpioSetPinState( HAL_GPIO_CS_SDCARD, HAL_GPIO_ACT )
/* sd card chip deselect & release spi resource */
#define sd_spi_deselect()                   hal_GpioSetPinState( HAL_GPIO_CS_SDCARD, HAL_GPIO_INACT )

/*--------------------------------------------------------------------------*/
/* Socket's Card-Present Pin                                                */
/* Socket's Write-Protection Pin                                            */
/*--------------------------------------------------------------------------*/
#define SD_SPI_WR_PROT                        HAL_GPIO_ACT
#define SD_SPI_WRITEABLE                    HAL_GPIO_INACT
#define SD_SPI_SOCKET_EMPTY                    HAL_GPIO_INACT
#define SD_SPI_CARD_INSERTED                HAL_GPIO_ACT

#define sd_spi_socket_is_empty( ptr )        hal_GpioGetPinState( HAL_GPIO_SDCARD_DET, ptr )
#define sd_spi_socket_is_wr_prot( ptr )        (*ptr = SD_SPI_WRITEABLE)

/*----------------------------------------------------------------------*/
/* Peripheral control                                                     */
/*----------------------------------------------------------------------*/
/* periph speed management */
#define SPI_SLOW_TARGET                        400000UL
#define SPI_FAST_TARGET                        16000000UL

/* automatic selection to be <= to the targeted speed */
#if (APB2_CLK * 100) / SPI_SLOW_TARGET <= 200UL
#define SPI_SLOW                            HAL_SPI_PRESC_2
#elif (APB2_CLK * 100) / SPI_SLOW_TARGET <= 400UL
#define SPI_SLOW                            HAL_SPI_PRESC_4
#elif (APB2_CLK * 100) / SPI_SLOW_TARGET <= 800UL
#define SPI_SLOW                            HAL_SPI_PRESC_8
#elif (APB2_CLK * 100) / SPI_SLOW_TARGET <= 1600UL
#define SPI_SLOW                            HAL_SPI_PRESC_16
#elif (APB2_CLK * 100) / SPI_SLOW_TARGET <= 3200UL
#define SPI_SLOW                            HAL_SPI_PRESC_32
#elif (APB2_CLK * 100) / SPI_SLOW_TARGET <= 6400UL
#define SPI_SLOW                            HAL_SPI_PRESC_64
#elif (APB2_CLK * 100) / SPI_SLOW_TARGET <= 12800UL
#define SPI_SLOW                            HAL_SPI_PRESC_128
#elif (APB2_CLK * 100) / SPI_SLOW_TARGET <= 25600UL
#define SPI_SLOW                            HAL_SPI_PRESC_256
#else
#error "Unable to achieve SPI_SLOW_TARGET"
#endif

/* automatic selection to be <= to the targeted speed */
#if (APB2_CLK * 100) / SPI_FAST_TARGET <= 200UL
#define SPI_FAST                            HAL_SPI_PRESC_2
#elif (APB2_CLK * 100) / SPI_FAST_TARGET <= 400UL
#define SPI_FAST                            HAL_SPI_PRESC_4
#elif (APB2_CLK * 100) / SPI_FAST_TARGET <= 800UL
#define SPI_FAST                            HAL_SPI_PRESC_8
#elif (APB2_CLK * 100) / SPI_FAST_TARGET <= 1600UL
#define SPI_FAST                            HAL_SPI_PRESC_16
#elif (APB2_CLK * 100) / SPI_FAST_TARGET <= 3200UL
#define SPI_FAST                            HAL_SPI_PRESC_32
#elif (APB2_CLK * 100) / SPI_FAST_TARGET <= 6400UL
#define SPI_FAST                            HAL_SPI_PRESC_64
#elif (APB2_CLK * 100) / SPI_FAST_TARGET <= 12800UL
#define SPI_FAST                            HAL_SPI_PRESC_128
#elif (APB2_CLK * 100) / SPI_FAST_TARGET <= 25600UL
#define SPI_FAST                            HAL_SPI_PRESC_256
#else
#error "Unable to achieve SPI_FAST_TARGET"
#endif

/* peripheral init */
#define sd_spi_init( speed )                hal_spiInit( SD_SPI_PORT, HAL_SPI_8b, HAL_SPI_Low, HAL_SPI_1Edge, HAL_SPI_SW, speed, HAL_SPI_MSB, 100 )

/* Transmit a byte to MMC via SPI */
#define sd_spi_xmit(dat)                    hal_spiReadWrite( SD_SPI_PORT, dat, NULL )

/* Receive a byte from MMC via SPI */
#define sd_spi_rcvr(ptr)                    hal_spiReadWrite( SD_SPI_PORT, 0xFF, ptr )

/* Transmit/Receive Block using DMA */
#define    SD_SPI_USE_BLOCK_ACCESS
#define sd_spi_block_read( ptr, len )        hal_spiReadBlock( SD_SPI_PORT, (unsigned short)(len), (unsigned char*)(ptr) )
#define sd_spi_block_write( ptr, len )        hal_spiWriteBlock( SD_SPI_PORT, (unsigned short)(len), (unsigned char*)(ptr) )

/*-----------------------------------------------------------------------*/
/* RTOS management                                                         */
/*-----------------------------------------------------------------------*/
#define sd_spi_take()                        hal_spiLock( SD_SPI_PORT )
#define sd_spi_give()                        hal_spiUnlock( SD_SPI_PORT )
#define sd_spi_yield()                        taskYIELD()

/* system tick hook */
void sd_spi_timerproc (void);

