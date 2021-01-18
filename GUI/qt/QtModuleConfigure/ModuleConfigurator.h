//<LICENSE>


// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MODULECONFIGURATOR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MODULECONFIGURATOR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef __MODULECONFIGURATOR_H
#define __MODULECONFIGURATOR_H
#include "QtModuleConfigure_global.h"

#include "ConfiguratorDialogBase.h"
#include <ConfigBase.h>
#include <base/timage.h>
#include <logging/logger.h>
#include <interactors/interactionbase.h>

#ifdef _MSC_VER // Shared object specific for msvc
typedef void * (__cdecl *FACTORY)(const char *, const char *, void *);
typedef int (__cdecl *DESTROYER)(const char *, void *);
#else // Shared object specific for gcc
typedef void * (*FACTORY)(const char *, const char *,void *) ;
typedef int (*DESTROYER)(const char *, void *);
typedef  void * HINSTANCE;
#define MODULECONFIGURATOR_API
#endif
// This class is exported from the ModuleConfigurator.dll
class QTMODULECONFIGURESHARED_EXPORT ModuleConfigurator {
protected:
	kipl::logging::Logger logger;

public:
    ModuleConfigurator(kipl::interactors::InteractionBase *interactor=nullptr);
	virtual ~ModuleConfigurator();
	void SetCurrentChain(std::list<ModuleConfig> &chain);
	bool configure(std::string application, std::string SharedObjectName, std::string ModuleName, std::map<std::string, std::string> &parameters);

protected:
    void loadDialog(std::string application, std::string sharedobjectname, std::string objectname);

    void destroy();
    virtual int GetImage(std::string sSelectedModule, kipl::interactors::InteractionBase *interactor=nullptr)=0;
	HINSTANCE hinstLib;
    FACTORY m_fnModuleFactory;
	DESTROYER m_fnDestroyer;

	std::string m_sApplication;
	std::string m_sSharedObject;
	std::string  m_sModuleName;
	ConfiguratorDialogBase *m_Dialog;
	int m_nCurrentItem;
	ConfigBase  *m_Config;

    kipl::interactors::InteractionBase *m_Interactor;
	kipl::base::TImage<float,3> m_Image;
    kipl::base::TImage<float,3> m_OriginalImage;
};

#endif
