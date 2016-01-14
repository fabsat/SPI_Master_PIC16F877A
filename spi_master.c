/******************************************************************************************
 * spi_master.c
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * PIC16F877A用SPI通信(Master)ソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 初版 || 2015/12/27 || Hirofumi Hamada
 *   初版作成
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(V3.10/Ubuntu)
 * XC8 (V1.35/Linux)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/

#include <xc.h>
#include "spi_master.h"
#include "pic_clock.h"


/* Prototype of static function */
static void spi_reset(void);


/*=====================================================
 * @breif
 *     SPI Master初期化関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     Pin43:SDO(RC5)
 *     Pin42:SDI(RC4/SDA)
 *     Pin37:SCK(RC3/SCL)
 *     Pin35:SS RC1(T1OSI/CCP2) *User Define
 *===================================================*/
void spi_master_init(void)
{
    /* SPI pin I/O Configuration */
    TRISC3 = 0;              // RC3 is SCK -> OUTPUT
    //TRISC4 = 1;              // RC4 is SDI -> INPUT
    TRISC5 = 0;              // RC5 is SDO -> OUTPUT
    TRISC1 = 0;              // RC1 is SS  -> OUTPUT

    /* Allow Programming of SPI configuring */
    SSPCONbits.SSPEN = 0;

    /*  SPI Mode Setup */
    SSPSTATbits.SMP = 0;    // Input data sampled at middle of data output time
    SSPCONbits.CKP  = 0;    // Idle state for clock is a low level
    SSPSTATbits.CKE = 0;    // Transmit occurs on transition from idle to active clock state
    
    /* SPI Master mode, clock = FOSC/4(Maximum speed) */
    SSPCONbits.SSPM3 = 0;          
    SSPCONbits.SSPM2 = 0;          
    SSPCONbits.SSPM1 = 0;          
    SSPCONbits.SSPM0 = 0;

    /* SS_PIN set HIGH */
    SS_PIN = 1;
   
    /* End SPI programming and Start serial port */
    SSPCONbits.SSPEN = 1;
}


/*=====================================================
 * @brief
 *     SPI Masterデータ受信関数(1Byte)
 * @param
 *     none
 * @return
 *     SSPBUF:読みだしたデータ(1Byte)
 *     -1    :timeout終了
 * @note
 *     ・通信完了フラグの待ち状態でreset_counterが
n *       0になったらresetをかける
 *     ・2回resetをかけても完了しなければtimeoutと
 *       して-1を返す
 *===================================================*/
char spi_master_receive(void)
{
    char dummy;
    unsigned char timeout_counter   = 0;
    unsigned char spi_reset_counter = 0xFF;

    /* Read data to dummy */
    dummy = SSPBUF;

    /* Slave Select -> Low */
    SS_PIN = 0;

    /* Set dummy data to SSPBUF ,SPI START */
    SSPBUF = 0xFF;

    /* Wait for receive finish */
    while(SSPSTATbits.BF == 0)
    {
        /* reset counter overflow sequence */
        if(spi_reset_counter == 0x00 && timeout_counter < 2)
        {
            spi_reset();                  // SPI reset
            timeout_counter++;            // Timeout count
            spi_reset_counter = 0xFF;     // Counter reset
            continue;
        }

        /* SPI timeout */
        if(timeout_counter == 2)
        {
            return -1;
        }

        /* reset counter decrement */
        spi_reset_counter--;       
    }

    /* Slave Select -> High */
    SS_PIN = 1;

    /* received data return */
    return SSPBUF;
}


/*=====================================================
 * @brief
 *     SPI Masterデータ送信関数(1Byte)
 * @param
 *     data:送信データ(1Byte)
 * @return
 *     0 :正常終了
 *     -1:timeout終了
 * @note
 *     ・通信完了フラグの待ち状態でreset_counterが
 *       0になったらresetをかける
 *     ・2回resetをかけても完了しなければtimeoutと
 *       して-1を返す
 *===================================================*/
char spi_master_send(char data)
{
    char dummy;    
    unsigned char timeout_counter   = 0;
    unsigned char spi_reset_counter = 0xFF;

    /* Read data to dummy */
    dummy = SSPBUF;

    /* Slave Select -> Low */
    SS_PIN = 0;

    /* Set dummy data to SSPBUF ,SPI START */
    SSPBUF = data;

    /* Wait for receive finish */
    while(SSPSTATbits.BF == 0)
    {
        /* reset counter overflow sequence */
        if(spi_reset_counter == 0x00 && timeout_counter < 2)
        {
            spi_reset();                  // SPI reset
            timeout_counter++;            // Timeout count
            spi_reset_counter = 0xFF;     // Counter reset
            continue;
        }

        /* SPI timeout */
        if(timeout_counter == 2)
        {
            return -1;
        }

        /* reset counter decrement */
        spi_reset_counter--;       
    }

    /* Slave Select -> High */
    SS_PIN = 1;
    
    /* Read data to dummy */
    dummy = SSPBUF;

    /* received data return */
    return 0;   
}


/*-----------------------------------------------------
 * @brief
 *     SPIリセット関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     待ち状態のreset_counterによって呼び出される
 *---------------------------------------------------*/
static void spi_reset(void)
{
    unsigned char dummy;
    
    SSPEN = 0;         //  Reset SPI module
    SSPEN = 1;         //  Reset SPI module
    dummy = SSPBUF;
    SSPIF = 0;
    SSPEN = 0;         //  Reset SPI module
    SSPEN = 1;         //  Reset SPI module
}
