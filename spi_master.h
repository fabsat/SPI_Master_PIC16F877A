/******************************************************************************************
 * spi_master.h
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * SPI通信(マスター側)ヘッダファイル
 *
 *=========================================================================================
 * ・ver1.00 || 初版 || 2015/12/27 || Hirofumi Hamada
 *   初版作成
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#ifndef _SPI_MASTER_H
#define	_SPI_MASTER_H

#include <xc.h>
#include "pic_clock.h"


/* Slave Select Pin */
#define SS_PIN (RC1)


/* Prototype of Function */
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
 *     Pin35:SS  RC1(T1OSI/CCP2)
 *===================================================*/
void spi_master_init(void);


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
char spi_master_receive(void);


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
char spi_master_send(char data);



#endif	/* _SPI_MASTER_H */

