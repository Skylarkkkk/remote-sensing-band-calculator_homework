#pragma once
#include <atlimage.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

class ImageData
{
private:
	int m_width; //图片宽度
	int m_height;//图片高度

	CImage m_image;       //整张图片
	CImage m_red_image;   //红波段图片
	CImage m_green_image; //绿波段图片
	CImage m_blue_image;  //蓝波段图片	
	CImage m_rag_image;   //红绿相加波段图片
	CImage m_rsg_image;   //红绿相减波段图片


public:
	//利用vector定义了二维动态数组
	vector<vector<int>> redBand;   //存储红波段数据
	vector<vector<int>> greenBand; //存储绿波段数据
	vector<vector<int>> blueBand;  //存储蓝波段数据
	vector<vector<int>> ragBand;   //存储红绿相加波段数据
	vector<vector<int>> rsgBand;   //存储红绿相减波段数据

public:
	ImageData(CString imgPath);    //构造函数
	void DisplayImage(CDC* pDC, CRect rect, char layer); //图片显示函数
	int GetWidth();  //获取图片宽度
	int GetHeight(); //获取图片高度
	void RedAddGreen(); //红绿相加
	void RedSubstractGreen(); //红绿相减
	double Calc_Band_Mean(vector<vector<int>> Band); //计算波段均值
	int Calc_Band_Min(vector<vector<int>> Band);  //计算波段最小值
	int Calc_Band_Max(vector<vector<int>> Band);  //计算波段最大值
	map <string, int>m_max;//  存储各波段最大值
	map <string, int>m_min;//  存储各波段最小值
	map <string, double>m_mean;//  存储各波段平均值
};
