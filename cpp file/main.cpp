/////////////////////////////////  Includes  //////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"
#include "resource.h"
#include<string.h>
#include<iostream>



//////////////////////////////// Implementation ///////////////////////////////

#pragma warning(suppress: 26461)
VOID WINAPI CompletionRoutine(_In_ DWORD dwErrorCode, _In_ DWORD dwNumberOfBytesTransfered, _Inout_ LPOVERLAPPED lpOverlapped) noexcept
{
  UNREFERENCED_PARAMETER(dwErrorCode);
  UNREFERENCED_PARAMETER(dwNumberOfBytesTransfered);
  UNREFERENCED_PARAMETER(lpOverlapped);
}

int _tmain()
{
  //Please note that all the following is just test code to exercise the CSerialPort API. It assumes
  //that a COM1 is available on the local computer.

    HANDLE hEvent = nullptr;
    constexpr const int nPortToUse = 5;
    CSerialPort port;
    port.Open(nPortToUse, 9600, CSerialPort::Parity::NoParity, 8, \
        CSerialPort::StopBits::OneStopBit, \
        CSerialPort::FlowControl::XonXoffFlowControl);

    UINT8 pszBuf[20];
    int weizhi = 0;
    UINT8 last = 0x0;
    //����ͨѶ  DD 00 55 00 00 00 00 00 00 00 00 00 00 00 00 CE 
    pszBuf[0] = 0xDD;
    pszBuf[1] = 0x00;
    for (int i = 3; i < 15; ++i) {
        pszBuf[i] = 0x00;
    }
    pszBuf[2] = 0x55;
    pszBuf[15] = 0xCE;

    //Sleep(3 * 1000);
    port.Write(pszBuf, static_cast<DWORD>(16 * sizeof(UINT8)));
    Sleep(1 * 1000);

    //����
    for (int j = 1; j < 13; j++) {
        weizhi = j * 3000000;
        //weizhi = (32830420+j * 570);
        if (weizhi > 36000000)
            weizhi = weizhi - 36000000;
        else if (weizhi < 0)
            weizhi = weizhi + 36000000;
        for (int i = 3; i < 7; ++i) {
            pszBuf[i] = weizhi % 256;
            weizhi = weizhi / 256;
        }//λ������
        //�豸��+���+������+�����12��+����
        //���   ���᣺0x01  ���᣺0x04   ���ߣ�0x03
        pszBuf[1] = 0x01; // ����
        pszBuf[2] = 0x0B;  // λ��ָ�����12λΪ λ��+����+���ٶȣ���3-14��
            //���ٶȺ��ٶ�����
        pszBuf[7] = 0x20;
        pszBuf[8] = 0xA1;
        pszBuf[9] = 0x07;
        pszBuf[10] = 0x00;//��������  ��ǰ�ߺ󣨲�����(5m/s)

        pszBuf[11] = 0x20;
        pszBuf[12] = 0xA1;
        pszBuf[13] = 0x07;
        pszBuf[14] = 0x00;//���ٶ�����(5m/s^2)

        //15λ���󷨡�
        last = 0x0;
        for (int i = 0; i < 15; i++) {
            last += pszBuf[i];
        }
        last = 0x100 - last;
        pszBuf[15] = last;

        port.Write(pszBuf, static_cast<DWORD>(16 * sizeof(UINT8)));
        Sleep(1 * 1000);

        //����
        for (int i = 3; i < 15; ++i) {
            pszBuf[i] = 0x00;
        }
        pszBuf[1] = 0x01;
        pszBuf[2] = 0x10;
        pszBuf[3] = 0x01;
        //pszBuf[7] = 0x01;

        last = 0x0;
        for (int i = 0; i < 15; i++) {
            last += pszBuf[i];
        }
        last = 0x100 - last;
        pszBuf[15] = last;

        Sleep(35 * 1000);
        port.Write(pszBuf, static_cast<DWORD>(16 * sizeof(UINT8)));
        
    }
    
    
    //�Ͽ�ͨѶ  DD 00 AA 00 00 00 00 00 00 00 00 00 00 00 00 79
    pszBuf[2] = 0xAA;
    pszBuf[0] = 0xDD;
    pszBuf[1] = 0x00;
    for (int i = 3; i < 15; ++i) {
        pszBuf[i] = 0x00;
    }
    pszBuf[15] = 0x79;

    
    port.Write(pszBuf, static_cast<DWORD>(16*sizeof(UINT8)));
    port.Close();

    //port.Write(pszBuf, static_cast<DWORD>(strlen(pszBuf)));

        //return FALSE;
    
    /*
    int a1 = 9000000;
    for (int i = 0; i < 4; ++i) {
        pszBuf[i] = a1 % 256;
        a1 = a1 / 256;
    }
    port.Write(pszBuf, static_cast<DWORD>(4 * sizeof(UINT8)));
    */

  return 0;
}