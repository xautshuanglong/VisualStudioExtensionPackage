// Package.h

#pragma once

#include <atlstr.h>
#include <VSLCommandTarget.h>


#include "resource.h"       // main symbols
#include "Guids.h"
#include "..\HelloWorldUI\Resource.h"

#include "..\HelloWorldUI\CommandIds.h"


#include "MyToolWindow.h"
#include <commctrl.h>


using namespace VSL;


class ATL_NO_VTABLE CHelloWorldPackage : 
	// CComObjectRootEx and CComCoClass are used to implement a non-thread safe COM object, and 
	// a partial implementation for IUnknown (the COM map below provides the rest).
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHelloWorldPackage, &CLSID_HelloWorld>,
	// Provides the implementation for IVsPackage to make this COM object into a VS Package.
	public IVsPackageImpl<CHelloWorldPackage, &CLSID_HelloWorld>,
	public IOleCommandTargetImpl<CHelloWorldPackage>,
	// Provides consumers of this object with the ability to determine which interfaces support
	// extended error information.
	public ATL::ISupportErrorInfoImpl<&__uuidof(IVsPackage)>
{
public:

// Provides a portion of the implementation of IUnknown, in particular the list of interfaces
// the CHelloWorldPackage object will support via QueryInterface
BEGIN_COM_MAP(CHelloWorldPackage)
	COM_INTERFACE_ENTRY(IVsPackage)
	COM_INTERFACE_ENTRY(IOleCommandTarget)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// COM objects typically should not be cloned, and this prevents cloning by declaring the 
// copy constructor and assignment operator private (NOTE:  this macro includes the declaration of
// a private section, so everything following this macro and preceding a public or protected 
// section will be private).
VSL_DECLARE_NOT_COPYABLE(CHelloWorldPackage)

public:
	CHelloWorldPackage():
		
		m_MyToolWindow(GetVsSiteCache())
	{
	}
	
	~CHelloWorldPackage()
	{
	}


	// This function provides the error information if it is not possible to load
	// the UI dll. It is for this reason that the resource IDS_E_BADINSTALL must
	// be defined inside this dll's resources.
	static const LoadUILibrary::ExtendedErrorInfo& GetLoadUILibraryErrorInfo()
	{
		static LoadUILibrary::ExtendedErrorInfo errorInfo(IDS_E_BADINSTALL);
		return errorInfo;
	}

	// DLL is registered with VS via a pkgdef file. Don't do anything if asked to
	// self-register.
	static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
	{
		return S_OK;
	}

// NOTE - the arguments passed to these macros can not have names longer then 30 characters
// Definition of the commands handled by this package
VSL_BEGIN_COMMAND_MAP()

    VSL_COMMAND_MAP_ENTRY(CLSID_HelloWorldCmdSet, cmdidShuanglongTest, NULL, CommandHandler::ExecHandler(&OnMyCommand))
    VSL_COMMAND_MAP_ENTRY(CLSID_HelloWorldCmdSet, cmdidShuanglongTool, NULL, CommandHandler::ExecHandler(&OnMyTool))
VSL_END_VSCOMMAND_MAP()


// The tool map implements IVsPackage::CreateTool that is called by VS to create a tool window 
// when appropriate.
VSL_BEGIN_TOOL_MAP()
    VSL_TOOL_ENTRY(CLSID_guidPersistanceSlot, m_MyToolWindow.CreateAndShow())
VSL_END_TOOL_MAP()

// Command handler called when the user selects the command to show the toolwindow.
void OnMyTool(CommandHandler* /*pSender*/, DWORD /*flags*/, VARIANT* /*pIn*/, VARIANT* /*pOut*/)
{
    m_MyToolWindow.CreateAndShow();
}

// Command handler called when the user selects the "My Command" command.
void OnMyCommand(CommandHandler* /*pSender*/, DWORD /*flags*/, VARIANT* /*pIn*/, VARIANT* /*pOut*/)
{
	// Get the string for the title of the message box from the resource dll.
	CComBSTR bstrTitle;
	VSL_CHECKBOOL_GLE(bstrTitle.LoadStringW(_AtlBaseModule.GetResourceInstance(), IDS_PROJNAME));
	// Get a pointer to the UI Shell service to show the message box.
	CComPtr<IVsUIShell> spUiShell = this->GetVsSiteCache().GetCachedService<IVsUIShell, SID_SVsUIShell>();
	LONG lResult;
	HRESULT hr = spUiShell->ShowMessageBox(
	                             0,
	                             CLSID_NULL,
	                             bstrTitle,
	                             W2OLE(L"Inside CHelloWorldPackage::Exec"),
	                             NULL,
	                             0,
	                             OLEMSGBUTTON_OK,
	                             OLEMSGDEFBUTTON_FIRST,
	                             OLEMSGICON_INFO,
	                             0,
	                             &lResult);
	VSL_CHECKHRESULT(hr);
}


private:
    HelloWorldToolWindow m_MyToolWindow;


};

// This exposes CHelloWorldPackage for instantiation via DllGetClassObject; however, an instance
// can not be created by CoCreateInstance, as CHelloWorldPackage is specifically registered with
// VS, not the the system in general.
OBJECT_ENTRY_AUTO(CLSID_HelloWorld, CHelloWorldPackage)
