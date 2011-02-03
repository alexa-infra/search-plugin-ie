This is a search plugin for IE (Internet Explorer) browser, written in C++, that embeds at context menu on selecting text, and redirects user to custom search engine.

It was written by Alexey Vasilyev (alexa.infra@gmail.com).

===============================================================================

Behavior:

# Plugin hasn't specific user interface like toolbars, panels, icons and buttons
# After install of plugin, it adds an item to context menu at browser pages (context menu for any text selection on web page)
# Context menu has a label with text like "Search by my super search engine.."
# When user clicks to context menu item:
## new browser tab or window is opened
## it points to URL with search phrase, URL format is configured as following "http://searchengine.com/?search=%s"
## search phrase is encoded by urlencode algorithm
## could also add it self version to search request
# if user selects more than MAX symbols, or less than MIN symbols at page, then after click on context menu message box is shown. MAX and MIN numbers are configured.
# at first run after installation, plugin opens its home page, url of page is configured

Supported versions:

# Windows XP+, 32bit
# IE 6+
# Included UAC enabled systems, and IE protected mode

Build code:

# Required software:
## Windows XP+
## Microsoft VS2008 or greater with Visual C++
## Microsoft Platform SDK
# Open solution, and build release and debug targets

Install/uninstall plugin:

Version of installed IE at system could be determined at the following regestry value
# HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Internet Explorer
# an item 'Version'

There are two ways to install/unistall plugin:
# use regsvr32 tool
## requires an administrator privilegies at system
## for install call command line: regsvr32.exe /i /s Plugin.dll
## for uninstall call: regsvr32.exe /u /s Plugin.dll
# direct call plugin registration
## load dynamically library Plugin.dll (by using LoadLibrary function)
## for install call DllRegisterServer function
## for uninstall call DllUnregisterServer

Build version and configure plugin:

# Open by MSVC file from project SearchPluginIE.rc
# Version could be changed at VS_VERSION_INFO
# Configuration params could be found at String table
# Note if you get the problems with displaying unicode symbols, then save rc file as unicode text.

Debug tips:

# build Debug target
# at project settings set up Debugging tool to "iexplore.exe" as start up application
## alternate: you could attach to existed IE process
# for enable debugging for IE7 or IE8, change the following registry value
## [HKEY_CURRENT_USER\Software\Microsoft\Internet Explorer\Main] "TabProcGrowth"=dword:0
# for deguggin of install/unistall process, change start up application at Debugging:
## for install: regsvr32 /i $(OutDir)\$(ProjectName).dll
## for uninstall: regsvr32 /u $(OutDir)\$(ProjectName).dll

Code notes:

# js-code of context menu item is placed at contextmenu.html
# the main function of COM-object is CPlugin::Navigate
# registration of COM-object is placed at Plugin.rgs
#* here params are setuped at code compilation, it could be found at Plugin.h after BEGIN_REGISTRY_MAP
# FirstRun feature processed
## functions DllUnregisterServer and DllRegisterServer at SearchPluginIE.cpp
## function CPlugin::SetSite (specially for IE protected mode)
## displaying of welcomepage is in CPlugin::OnBeforeNavigate
# open new tab - CPlugin::NavigateBrowser

===============================================================================

Happy IE programming :)

Sincerely, 
Alexey Vasilyev
