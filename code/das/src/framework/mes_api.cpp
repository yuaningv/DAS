#include "mes_api.h"
#include "MES.h"


MES* g_pMES = NULL;

MES_API* MES_API::m_pMes = nullptr;

MES_API* MES_API::GetInstance()
{
    if (nullptr == m_pMes)
    {
        m_pMes = new MES_API();
    }

    return m_pMes;
}

MES_API::MES_API()
{
    g_pMES = new MES;
}

MES_API::~MES_API()
{
    if (g_pMES != NULL)
    {
        delete g_pMES;
        g_pMES = NULL;
    }
}


void MES_API::show()
{
    g_pMES->activateWindow();
    g_pMES->showMaximized();
}

