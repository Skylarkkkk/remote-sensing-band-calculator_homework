#pragma once
#include <atlimage.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

class ImageData
{
private:
	int m_width; //ͼƬ���
	int m_height;//ͼƬ�߶�

	CImage m_image;       //����ͼƬ
	CImage m_red_image;   //�첨��ͼƬ
	CImage m_green_image; //�̲���ͼƬ
	CImage m_blue_image;  //������ͼƬ	
	CImage m_rag_image;   //������Ӳ���ͼƬ
	CImage m_rsg_image;   //�����������ͼƬ


public:
	//����vector�����˶�ά��̬����
	vector<vector<int>> redBand;   //�洢�첨������
	vector<vector<int>> greenBand; //�洢�̲�������
	vector<vector<int>> blueBand;  //�洢����������
	vector<vector<int>> ragBand;   //�洢������Ӳ�������
	vector<vector<int>> rsgBand;   //�洢���������������

public:
	ImageData(CString imgPath);    //���캯��
	void DisplayImage(CDC* pDC, CRect rect, char layer); //ͼƬ��ʾ����
	int GetWidth();  //��ȡͼƬ���
	int GetHeight(); //��ȡͼƬ�߶�
	void RedAddGreen(); //�������
	void RedSubstractGreen(); //�������
	double Calc_Band_Mean(vector<vector<int>> Band); //���㲨�ξ�ֵ
	int Calc_Band_Min(vector<vector<int>> Band);  //���㲨����Сֵ
	int Calc_Band_Max(vector<vector<int>> Band);  //���㲨�����ֵ
	map <string, int>m_max;//  �洢���������ֵ
	map <string, int>m_min;//  �洢��������Сֵ
	map <string, double>m_mean;//  �洢������ƽ��ֵ
};
