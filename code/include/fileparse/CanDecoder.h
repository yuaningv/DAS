#ifndef __CAN_DECODER_H__
#define __CAN_DECODER_H__

#include <string>

using namespace std;

typedef double(*Func_GetCanValue)(unsigned char* pBuffer, int pos);

enum CANDATATYPE
{
	CANDATATYPE_NONE,
	CANDATATYPE_BOOL,
	CANDATATYPE_BYTE,
	CANDATATYPE_WORD,
	CANDATATYPE_FLOAT,
	CANDATATYPE_MBIT,
};

class DLL_EXPORT CCanItemData
{
public:
	CCanItemData()
	{
		m_Name = "";
		m_DispName = "";
		m_Address = 0;
		m_DataType = 0;

		m_minValue = 0.0;
		m_maxValue = 0.0;

		m_fGetValue = NULL;
		m_fParam = 0;
	}
	virtual ~CCanItemData(){}
	
public:
    string		m_Name;
    string		m_DispName;
    unsigned int m_Address;
    unsigned int m_DataType;
    double		m_minValue;
    double		m_maxValue;

	Func_GetCanValue m_fGetValue;
	int				 m_fParam;
};

class DLL_EXPORT CCanData
{
public:
	CCanData()
	{
		m_pCanItem = NULL;
		m_CurTime = 0;
		m_Value = 0.0;
	}
	virtual ~CCanData(){}

    CCanItemData* m_pCanItem;
    unsigned long m_CurTime; //sec.msecs
    double m_Value;
};

class CCanDataProfilePrivateData;
class DLL_EXPORT CCanDataProfile
{
public:
	CCanDataProfile();
	virtual ~CCanDataProfile();

public:
	int Init(const char* configFile=NULL);
	int DeInit();
	
	int GetItemCount();
	CCanItemData* GetCanItem(int index);

private:    
	CCanDataProfilePrivateData* m_data;
};

class CCanIndex;
class CCanDataDecoderPrivateData;
class CCanDataDecoder
{
public:
	CCanDataDecoder();
	virtual ~CCanDataDecoder();

public:
	int Init(CCanDataProfile* profile);
	int DeInit();
	int ParseData(unsigned char* pBuffer, unsigned int uSecs, CCanData* pDstData/*in/out*/, int iCount);
	int ParseData(unsigned char* pBuffer, unsigned int length, unsigned int uSecs, CCanData* pDstData/*in/out*/, int iCount);

private:
	CCanIndex* GetCanIndex(unsigned int uAddress);

private:
	CCanDataDecoderPrivateData* m_data;
};

#endif