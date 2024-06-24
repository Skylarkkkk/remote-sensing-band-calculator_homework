#include "pch.h"
#include "ImageData.h"
#include <atlimage.h>
#include <algorithm>
#include <cmath>


/// <summary>
/// ���캯�����Ը���ֵ���г�ʼ������������ͼ�񣬷ֳɺ�������������
/// </summary>
/// <param name="imagePath"></param>
ImageData::ImageData(CString imgPath)
{
	HRESULT hr = m_image.Load(imgPath);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("Failed to load image"));
		return;
	}
	AfxMessageBox(_T("Image successfully loaded"));

	m_width = m_image.GetWidth();
	m_height = m_image.GetHeight();

	// ��ȡ�첨������
	redBand = vector<vector<int>>(m_width, std::vector<int>(m_height));

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			COLORREF color = m_image.GetPixel(x, y);
			redBand[x][y] = GetRValue(color);
		}
	}
	m_max.insert(make_pair("Red", Calc_Band_Max(redBand)));
	m_min.insert(make_pair("Red", Calc_Band_Min(redBand)));
	m_mean.insert(make_pair("Red", Calc_Band_Mean(redBand)));

	m_red_image.Create(m_width, m_height, m_image.GetBPP());

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			m_red_image.SetPixel(x, y, RGB(redBand[x][y], 0, 0));
		}
	}

	// ��ȡ�̲�������
	greenBand = vector<vector<int>>(m_width, std::vector<int>(m_height));

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			COLORREF color = m_image.GetPixel(x, y);
			greenBand[x][y] = GetGValue(color);
		}
	}

	m_max.insert(make_pair("Green", Calc_Band_Max(greenBand)));
	m_min.insert(make_pair("Green", Calc_Band_Min(greenBand)));
	m_mean.insert(make_pair("Green", Calc_Band_Mean(greenBand)));

	m_green_image.Create(m_width, m_height, m_image.GetBPP());


	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			m_green_image.SetPixel(x, y, RGB(0, greenBand[x][y], 0));
		}
	}

	// ��ȡ����������
	blueBand = vector<vector<int>>(m_width, std::vector<int>(m_height));

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			COLORREF color = m_image.GetPixel(x, y);
			blueBand[x][y] = GetBValue(color);
		}
	}
	m_max.insert(make_pair("Blue", Calc_Band_Max(blueBand)));
	m_min.insert(make_pair("Blue", Calc_Band_Min(blueBand)));
	m_mean.insert(make_pair("Blue", Calc_Band_Mean(blueBand)));

	m_blue_image.Create(m_width, m_height, m_image.GetBPP());


	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			m_blue_image.SetPixel(x, y, RGB(0, 0, blueBand[x][y]));
		}
	}

}






/// <summary>
/// ��picture control����ʾͼƬ
/// </summary>
/// <param name="pDC"></param>
/// <param name="rect"></param>
/// <param name="layer"></param>
void ImageData::DisplayImage(CDC* pDC, CRect rect, char layer)
{
	switch (layer)
	{
	case 'r': //�첨��
		m_red_image.Draw(pDC->m_hDC, rect);
		break;
	case 'g': //�̲���
		m_green_image.Draw(pDC->m_hDC, rect);
		break;
	case 'b': //������
		m_blue_image.Draw(pDC->m_hDC, rect);
		break;
	case 'a': //�������
		m_rag_image.Draw(pDC->m_hDC, rect);
		break;
	case 's': //�������
		m_rsg_image.Draw(pDC->m_hDC, rect);
		break;
	}
}

int ImageData::GetWidth()
{
	return m_width;
}

int ImageData::GetHeight()
{
	return m_height;
}

void ImageData::RedAddGreen()
{
	ragBand = vector<vector<int>>(m_width, vector<int>(m_height));

	m_rag_image.Create(m_width, m_height, m_image.GetBPP());

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			ragBand[x][y] = redBand[x][y] + greenBand[x][y] > 255 ? 255 : redBand[x][y] + greenBand[x][y];
			ragBand[x][y] = static_cast<int>(255 * pow(ragBand[x][y] / 255.0, 2));   //GammaУ��
			m_rag_image.SetPixel(x, y, RGB(ragBand[x][y], ragBand[x][y], ragBand[x][y]));
		}
	}

	m_max.insert(make_pair("a", Calc_Band_Max(ragBand)));
	m_min.insert(make_pair("a", Calc_Band_Min(ragBand)));
	m_mean.insert(make_pair("a", Calc_Band_Mean(ragBand)));

}

void ImageData::RedSubstractGreen()
{
	rsgBand = vector<vector<int>>(m_width, vector<int>(m_height));

	m_rsg_image.Create(m_width, m_height, m_image.GetBPP());

	
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			rsgBand[x][y] = redBand[x][y] - greenBand[x][y];
		}
	}

	double minval = Calc_Band_Min(rsgBand);
	double maxval = Calc_Band_Max(rsgBand);

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			rsgBand[x][y] = static_cast<int>(255 * (rsgBand[x][y] - minval) / (maxval - minval));  //����У��  
		}
	}
	

	m_max.insert(make_pair("s", Calc_Band_Max(rsgBand)));
	m_min.insert(make_pair("s", Calc_Band_Min(rsgBand)));
	m_mean.insert(make_pair("s", Calc_Band_Mean(rsgBand)));

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			m_rsg_image.SetPixel(x, y, RGB(rsgBand[x][y], rsgBand[x][y], rsgBand[x][y]));
		}
	}
}

//���㲨�ξ�ֵ
double ImageData::Calc_Band_Mean(vector<vector<int>> Band)
{
	double sum = 0;
	for (auto row : Band)
	{
		for (int value : row)
		{
			sum += value;
		}
	}
	return sum / (m_width * m_height);
}

//���㲨����Сֵ
int ImageData::Calc_Band_Min(vector<vector<int>> Band)
{
	int min = 999;
	for (auto row : Band)
	{
		for (int value : row)
		{
			min = value < min ? value : min;
		}
	}
	return min;
}

//���㲨�����ֵ
int ImageData::Calc_Band_Max(vector<vector<int>> Band)
{
	int max = 0;
	for (auto row : Band)
	{
		for (int value : row)
		{
			max = value > max ? value : max;
		}
	}
	return max;
}