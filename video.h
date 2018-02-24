#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//��������� ������
struct SVideo_DisplaySetting
{
 int ScreenWidth;//������ ������
 int ScreenHeight;//������ ������
 int LineWidth;//����� �����
 unsigned char *VPtr;//��������� �� ����������
 int DVPtr;//��������� ������ ��� �������� �� y
};

//��������� TGA-�����
#pragma pack(1)
struct TGAHEADER
{
 char identsize;//������ ���� ID ��������� (0)
 char colorMapType;//���� �� �������:0-���,1-����
 char imageType;//��� ��������:0-���,1-��������� �����,2-RGB,3-������� ������, (3-� ��� - RLE-�����������)
 unsigned short	colorMapStart;//������ ����� ������
 unsigned short	colorMapLength;//���������� ������ � �����
 unsigned char 	colorMapBits;//����������� �������
 unsigned short	xstart;//��������� ���������� �����������
 unsigned short	ystart;
 unsigned short	width;//������ ����������� �� X
 unsigned short	height;//������ ����������� �� Y
 char bits;//��������� ��� �� ������� (8,16,24,32)
 char descriptor;//���������� ������������
};
#pragma pack()

#endif // VIDEO_H_INCLUDED