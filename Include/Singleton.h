#ifndef SINGLETON_H_
#define SINGLETON_H_
#ifdef WIN32
#else
#include <stddef.h>
#endif // WIN32

template<class T>
class Singleton
{
public:
	static T& Instance()
	{ 
		if(NULL == m_pInstance)
		{
			m_pInstance = new T;
		}
		return *m_pInstance;
	}
	static void UnInstance()
	{
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
protected:
	Singleton(){};
private:
	static T* m_pInstance;
 };

template<class T>
T*	Singleton<T>::m_pInstance	=	NULL;

#endif // SINGLETON_H_
